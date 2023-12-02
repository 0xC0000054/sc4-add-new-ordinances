////////////////////////////////////////////////////////////////////////////
//
// This file is part of sc4-add-new-ordinances, a DLL Plugin for SimCity 4
// that experiments with adding new ordinances to the game.
//
// Copyright (c) 2023 Nicholas Hayes
//
// This file is licensed under terms of the MIT License.
// See LICENSE.txt for more information.
//
////////////////////////////////////////////////////////////////////////////

#include "version.h"
#include "Logger.h"
#include "TestOrdinance.h"
#include "cIGZFrameWork.h"
#include "cIGZApp.h"
#include "cISC4App.h"
#include "cISC4City.h"
#include "cISC4Ordinance.h"
#include "cISC4OrdinanceSimulator.h"
#include "cISC4ResidentialSimulator.h"
#include "cISC4Simulator.h"
#include "cIGZMessageServer2.h"
#include "cIGZMessageTarget.h"
#include "cIGZMessageTarget2.h"
#include "cIGZString.h"
#include "cRZMessage2COMDirector.h"
#include "cRZMessage2Standard.h"
#include "cRZBaseString.h"
#include "GZServPtrs.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <Windows.h>
#include "wil/resource.h"
#include "wil/win32_helpers.h"

static constexpr uint32_t kSC4MessagePostCityInit = 0x26d31ec1;
static constexpr uint32_t kSC4MessagePreCityShutdown = 0x26D31EC2;

static constexpr uint32_t kNewOrdinanceTestPluginDirectorID = 0xc8f8cd0f;

static constexpr std::string_view PluginLogFileName = "SC4AddNewOrdinance.log";

class cGZNewOrdinanceTestDllDirector : public cRZMessage2COMDirector
{
public:

	cGZNewOrdinanceTestDllDirector()
		: testOrdinance(0x7faee31b, "New ordinance test1", "Test1", CreateOrdinanceEffects())
	{
		std::filesystem::path logFilePath = GetDllFolderPath();
		logFilePath /= PluginLogFileName;

		Logger& logger = Logger::GetInstance();
		logger.Init(logFilePath, LogOptions::All);
		logger.WriteLogFileHeader("SC4AddNewOrdinance v" PLUGIN_VERSION_STR);
	}

	uint32_t GetDirectorID() const
	{
		return kNewOrdinanceTestPluginDirectorID;
	}

	void EnumClassObjects(cIGZCOMDirector::ClassObjectEnumerationCallback pCallback, void* pContext)
	{
		// The classes you want to add must be initialized in the DLL constructor because
		// the framework calls this method before OnStart or any of the hook callbacks.
		// This method is called once when initializing a director, the list of class IDs
		// it returns is cached by the framework.
		pCallback(testOrdinance.GetID(), 0, pContext);
	}

	bool GetClassObject(uint32_t rclsid, uint32_t riid, void** ppvObj)
	{
		// To retrieve an instance of a registered class the framework will call the
		// GetClassObject method whenever it needs the director to provide one.

		bool result = false;

		if (rclsid == testOrdinance.GetID())
		{
			result = testOrdinance.QueryInterface(riid, ppvObj);
		}

		return result;
	}

	OrdinancePropertyHolder CreateOrdinanceEffects()
	{
		std::vector<cSCBaseProperty> properties;

		properties.push_back(cSCBaseProperty(0xaa5b8407, 0)); // Mayor rating
		properties.push_back(cSCBaseProperty(0x2a633000, 1.0f)); // Commercial Demand Effect

		cSCBaseProperty travelStrategyModifier(0x8a612fee);
		cIGZVariant* travelStrategyVariant = travelStrategyModifier.GetPropertyValue();

		std::array<int32_t, 9> travelStrategyModifierData
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		travelStrategyVariant->RefSint32(travelStrategyModifierData.data(), travelStrategyModifierData.size());
		properties.push_back(travelStrategyModifier);

		cSCBaseProperty airEffectByZoneType(0x8a67e373);
		cIGZVariant* airEffectVariant = airEffectByZoneType.GetPropertyValue();

		std::array<float, 16> airEffectData
		{
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		};

		airEffectVariant->RefFloat32(airEffectData.data(), airEffectData.size());
		properties.push_back(airEffectByZoneType);

		return OrdinancePropertyHolder(properties);
	}

	void PostCityInit(cIGZMessage2Standard* pStandardMsg)
	{
		cISC4City* pCity = reinterpret_cast<cISC4City*>(pStandardMsg->GetIGZUnknown());

		if (pCity)
		{
			cISC4OrdinanceSimulator* pOrdinanceSimulator = pCity->GetOrdinanceSimulator();

			if (pOrdinanceSimulator)
			{
				testOrdinance.PostCityInit(pCity);

				// Only add the ordinance if it is not already present. If it is part
				// of the city save file it will have already been loaded at this point.
				if (!pOrdinanceSimulator->GetOrdinanceByID(testOrdinance.GetID()))
				{
					pOrdinanceSimulator->AddOrdinance(testOrdinance);
				}

				DumpRegisteredOrdinances(pCity, pOrdinanceSimulator);
			}
		}
	}

	void PreCityShutdown(cIGZMessage2Standard* pStandardMsg)
	{
		cISC4City* pCity = reinterpret_cast<cISC4City*>(pStandardMsg->GetIGZUnknown());

		if (pCity)
		{
			cISC4OrdinanceSimulator* pOrdinanceSimulator = pCity->GetOrdinanceSimulator();

			if (pOrdinanceSimulator)
			{
				testOrdinance.PreCityShutdown(pCity);
				pOrdinanceSimulator->RemoveOrdinance(testOrdinance);
			}
		}
	}

	bool DoMessage(cIGZMessage2* pMessage)
	{
		cIGZMessage2Standard* pStandardMsg = static_cast<cIGZMessage2Standard*>(pMessage);
		uint32_t dwType = pMessage->GetType();

		switch (dwType)
		{
		case kSC4MessagePostCityInit:
			PostCityInit(pStandardMsg);
			break;
		case kSC4MessagePreCityShutdown:
			PreCityShutdown(pStandardMsg);
			break;
		}

		return true;
	}

	bool PostAppInit()
	{
		Logger& logger = Logger::GetInstance();

		cIGZMessageServer2Ptr pMsgServ;
		if (pMsgServ)
		{
			std::vector<uint32_t> requiredNotifications;
			requiredNotifications.push_back(kSC4MessagePostCityInit);
			requiredNotifications.push_back(kSC4MessagePreCityShutdown);

			for (uint32_t messageID : requiredNotifications)
			{
				if (!pMsgServ->AddNotification(this, messageID))
				{
					logger.WriteLine(LogOptions::Errors, "Failed to subscribe to the required notifications.");
					return true;
				}
			}
		}
		else
		{
			logger.WriteLine(LogOptions::Errors, "Failed to subscribe to the required notifications.");
			return true;
		}
		return true;
	}

	bool OnStart(cIGZCOM* pCOM)
	{
		cIGZFrameWork* const pFramework = RZGetFrameWork();

		if (pFramework->GetState() < cIGZFrameWork::kStatePreAppInit)
		{
			pFramework->AddHook(this);
		}
		else
		{
			PreAppInit();
		}
		return true;
	}

private:
	std::filesystem::path GetDllFolderPath()
	{
		wil::unique_cotaskmem_string modulePath = wil::GetModuleFileNameW(wil::GetModuleInstanceHandle());

		std::filesystem::path temp(modulePath.get());

		return temp.parent_path();
	}

	void DumpRegisteredOrdinances(cISC4City* pCity, cISC4OrdinanceSimulator* pOrdinanceSimulator)
	{
		Logger& logger = Logger::GetInstance();

		if (!logger.IsEnabled(LogOptions::DumpRegisteredOrdinances))
		{
			return;
		}

		uint32_t dwCountOut = 0;

		uint32_t registeredOrdinances = pOrdinanceSimulator->GetOrdinanceIDArray(nullptr, dwCountOut);

		logger.WriteLineFormatted(
			LogOptions::DumpRegisteredOrdinances,
			"The game has %d ordinances registered.",
			registeredOrdinances);

		if (registeredOrdinances > 0)
		{
			std::vector<uint32_t> registeredOrdinanceIDs(static_cast<size_t>(registeredOrdinances));
			uint32_t ordinancesRequested = registeredOrdinances;

			uint32_t ordinancesFetched = pOrdinanceSimulator->GetOrdinanceIDArray(registeredOrdinanceIDs.data(), ordinancesRequested);

			if (ordinancesFetched > 0)
			{
				int32_t cityPopulation = -1;
				cISC4ResidentialSimulator* pResidentialSimulator = pCity->GetResidentialSimulator();
				if (pResidentialSimulator)
				{
					cityPopulation = pResidentialSimulator->GetPopulation();
				}

				for (uint32_t i = 0; i < ordinancesFetched; i++)
				{
					uint32_t clsid = registeredOrdinanceIDs[i];

					cISC4Ordinance* pOrdinance = pOrdinanceSimulator->GetOrdinanceByID(clsid);

					if (pOrdinance)
					{
						uint32_t id = pOrdinance->GetID();
						cIGZString* name = pOrdinance->GetName();
						bool isIncome = pOrdinance->IsIncomeOrdinance();
						int64_t enactmentIncome = pOrdinance->GetEnactmentIncome();
						int64_t retracmentIncome = pOrdinance->GetRetracmentIncome();
						int64_t monthlyConstantIncome = pOrdinance->GetMonthlyConstantIncome();
						float monthlyIncomeFactor = pOrdinance->GetMonthlyIncomeFactor();
						int64_t currentMonthlyIncome = pOrdinance->GetCurrentMonthlyIncome();

						if (name)
						{
							logger.WriteLineFormatted(
								LogOptions::DumpRegisteredOrdinances,
								"0x%08x = %s, income=%s, enactment=%lld, retracment=%lld, monthly: constant=%lld, factor=%f, current=%lld, city population=%d",
								clsid,
								name->ToChar(),
								isIncome ? "true" : "false",
								enactmentIncome,
								retracmentIncome,
								monthlyConstantIncome,
								monthlyIncomeFactor,
								currentMonthlyIncome,
								cityPopulation);
						}
						else
						{
							logger.WriteLineFormatted(LogOptions::DumpRegisteredOrdinances, "0x%08x", clsid);
						}
					}
					else
					{
						logger.WriteLineFormatted(LogOptions::DumpRegisteredOrdinances, "0x%08x", clsid);
					}
				}
			}
		}
	}

	TestOrdinance testOrdinance;
};

cRZCOMDllDirector* RZGetCOMDllDirector() {
	static cGZNewOrdinanceTestDllDirector sDirector;
	return &sDirector;
}