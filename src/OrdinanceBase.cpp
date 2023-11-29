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

#include "OrdinanceBase.h"
#include "cIGZDate.h"
#include "cISC4ResidentialSimulator.h"
#include "cISC4Simulator.h"
#include "SC4Percentage.h"
#include <algorithm>
#include <stdlib.h>

namespace
{
	bool ApproximatelyEqual(double a, double b, double tolerance)
	{
		// Adapted from https://stackoverflow.com/a/253874
		// It is based on an algorithm in "The Art of Computer Programming: Seminumerical algorithms" by Donald Knuth
		return abs(a - b) <= ((abs(a) < abs(b) ? abs(b) : abs(a)) * tolerance);
	}
}

OrdinanceBase::OrdinanceBase(uint32_t clsid, const char* name, const char* description)
: clsid(clsid),
  refCount(0),
  name(name),
  description(description),
  monthlyAdjustedIncome(0),
  available(false),
  on(false),
  enabled(true),
  pResidentialSimulator(nullptr),
  pSimulator(nullptr),
  miscProperties(),
  logger(Logger::GetInstance())
{
}

OrdinanceBase::OrdinanceBase(
	uint32_t clsid, 
	const char* name,
	const char* description,
	const OrdinancePropertyHolder& properties)
	: clsid(clsid),
	  refCount(0),
	  name(name),
	  description(description),
	  monthlyAdjustedIncome(0),
	  available(false),
	  on(false),
	  enabled(true),
	  pResidentialSimulator(nullptr),
	  pSimulator(nullptr),
	  miscProperties(properties),
	  logger(Logger::GetInstance())
{
}

bool OrdinanceBase::QueryInterface(uint32_t riid, void** ppvObj)
{
	if (riid == clsid)
	{
		*ppvObj = this;
		AddRef();

		return true;
	}
	else if (riid == GZIID_cIGZUnknown)
	{
		*ppvObj = static_cast<cIGZUnknown*>(this);
		AddRef();

		return true;
	}

	return false;
}

uint32_t OrdinanceBase::AddRef()
{
	return ++refCount;
}

uint32_t OrdinanceBase::Release()
{
	if (refCount > 0)
	{
		--refCount;
	}
	return refCount;
}

bool OrdinanceBase::Init(void)
{
	enabled = true;
	return true;
}

bool OrdinanceBase::Shutdown(void)
{
	enabled = false;
	return true;
}

int64_t OrdinanceBase::GetCurrentMonthlyIncome(void)
{
	const int64_t monthlyConstantIncome = GetMonthlyConstantIncome();
	const double monthlyIncomeFactor = GetMonthlyIncomeFactor();

	// The monthly income factor is multiplied by the city population.
	const int32_t cityPopulation = pResidentialSimulator->GetPopulation();
	const double populationIncome = monthlyIncomeFactor * static_cast<double>(cityPopulation);

	const double monthlyIncome = static_cast<double>(monthlyConstantIncome) + populationIncome;	

	int64_t monthlyIncomeInteger = 0;

	if (monthlyIncome < std::numeric_limits<int64_t>::min())
	{
		monthlyIncomeInteger = std::numeric_limits<int64_t>::min();
	}
	else if (monthlyIncome > std::numeric_limits<int64_t>::max())
	{
		monthlyIncomeInteger = std::numeric_limits<int64_t>::max();
	}
	else
	{
		monthlyIncomeInteger = static_cast<int64_t>(monthlyIncome);
	}

	logger.WriteLineFormatted(
		LogOptions::OrdinanceAPI,
		"%s: monthly income: constant=%lld, factor=%f, population=%d, current=%lld",
		__FUNCTION__,
		monthlyConstantIncome,
		monthlyIncomeFactor,
		cityPopulation,
		monthlyIncomeInteger);

	return monthlyIncomeInteger;
}

uint32_t OrdinanceBase::GetID(void)
{
	return clsid;
}

cIGZString* OrdinanceBase::GetName(void)
{
	return &name;
}

cIGZString* OrdinanceBase::GetDescription(void)
{
	return &description;
}

uint32_t OrdinanceBase::GetYearFirstAvailable(void)
{
	return 0;
}

SC4Percentage OrdinanceBase::GetChanceAvailability(void)
{
	SC4Percentage percentage{ 100.0f };

	return percentage;
}

cISCPropertyHolder* OrdinanceBase::GetMiscProperties()
{
	return &miscProperties;
}

uint32_t OrdinanceBase::GetAdvisorID(void)
{
	return 0;
}

bool OrdinanceBase::IsAvailable(void)
{
	return available;
}

bool OrdinanceBase::IsOn(void)
{
	return available && on;
}

bool OrdinanceBase::IsEnabled(void)
{
	return enabled;
}

int64_t OrdinanceBase::GetMonthlyAdjustedIncome(void)
{
	logger.WriteLineFormatted(
		LogOptions::OrdinanceAPI,
		"%s: result=%lld",
		__FUNCTION__,
		monthlyAdjustedIncome);

	return monthlyAdjustedIncome;
}

bool OrdinanceBase::CheckConditions(void)
{
	bool result = false;

	if (enabled)
	{
		if (pSimulator)
		{
			cIGZDate* simDate = pSimulator->GetSimDate();

			if (simDate)
			{
				result = simDate->Year() >= GetYearFirstAvailable();
			}
		}
	}

	logger.WriteLineFormatted(
		LogOptions::OrdinanceAPI,
		"%s: result=%d",
		__FUNCTION__,
		result);

	return result;
}

bool OrdinanceBase::Simulate(void)
{
	monthlyAdjustedIncome = GetCurrentMonthlyIncome();

	logger.WriteLineFormatted(
		LogOptions::OrdinanceAPI,
		"%s: monthlyAdjustedIncome=%lld",
		__FUNCTION__,
		monthlyAdjustedIncome);

	return true;
}

bool OrdinanceBase::SetAvailable(bool isAvailable)
{
	logger.WriteLineFormatted(
		LogOptions::OrdinanceAPI,
		"%s: value=%d",
		__FUNCTION__,
		isAvailable);

	available = isAvailable;
	monthlyAdjustedIncome = 0;
	return true;
}

bool OrdinanceBase::SetOn(bool isOn)
{
	logger.WriteLineFormatted(
		LogOptions::OrdinanceAPI,
		"%s: value=%d",
		__FUNCTION__,
		isOn);

	on = isOn;
	return true;
}

bool OrdinanceBase::SetEnabled(bool isEnabled)
{
	logger.WriteLineFormatted(
		LogOptions::OrdinanceAPI,
		"%s: value=%d",
		__FUNCTION__,
		isEnabled);

	enabled = isEnabled;
	return true;
}

bool OrdinanceBase::ForceAvailable(bool isAvailable)
{
	return SetAvailable(isAvailable);
}

bool OrdinanceBase::ForceOn(bool isOn)
{
	return SetOn(isOn);
}

bool OrdinanceBase::ForceEnabled(bool isEnabled)
{
	return SetEnabled(isEnabled);
}

bool OrdinanceBase::ForceMonthlyAdjustedIncome(int64_t monthlyAdjustedIncome)
{
	logger.WriteLineFormatted(
		LogOptions::OrdinanceAPI,
		"%s: value=%lld",
		__FUNCTION__,
		monthlyAdjustedIncome);

	monthlyAdjustedIncome = monthlyAdjustedIncome;
	return true;
}

bool OrdinanceBase::PostCityInit(cISC4City* pCity)
{
	bool result = false;

	if (pCity)
	{
		pResidentialSimulator = pCity->GetResidentialSimulator();
		pSimulator = pCity->GetSimulator();

		if (pResidentialSimulator && pSimulator)
		{
			result = Init();
		}
	}

	return result;
}

bool OrdinanceBase::PreCityShutdown(cISC4City* pCity)
{
	bool result = Shutdown();

	pResidentialSimulator = nullptr;
	pSimulator = nullptr;
	miscProperties.RemoveAllProperties();

	return result;
}
