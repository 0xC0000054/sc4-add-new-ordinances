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

#pragma once
#include "cISC4Ordinance.h"
#include "cISC4City.h"
#include "cIGZSerializable.h"
#include "cRZBaseString.h"
#include "OrdinancePropertyHolder.h"
#include "Logger.h"

class OrdinanceBase : public cISC4Ordinance, cIGZSerializable
{
public:

	OrdinanceBase(
		uint32_t clsid,
		const char* name,
		const char* description,
		int64_t enactmentIncome,
		int64_t retracmentIncome,
		int64_t monthlyConstantIncome,
		float monthlyIncomeFactor,
		bool isIncomeOrdinance);

	OrdinanceBase(
		uint32_t clsid,
		const char* name,
		const char* description,
		int64_t enactmentIncome,
		int64_t retracmentIncome,
		int64_t monthlyConstantIncome,
		float monthlyIncomeFactor,
		bool isIncomeOrdinance,
		const OrdinancePropertyHolder& properties);

	OrdinanceBase(const OrdinanceBase& other);
	OrdinanceBase(OrdinanceBase&& other) noexcept;

	OrdinanceBase& operator=(const OrdinanceBase& other);
	OrdinanceBase& operator=(OrdinanceBase&& other) noexcept;

	bool QueryInterface(uint32_t riid, void** ppvObj);
	
	uint32_t AddRef();

	uint32_t Release();

	bool Init(void);

	bool Shutdown(void);

	virtual int64_t GetCurrentMonthlyIncome(void);

	uint32_t GetID(void) const;
	
	cIGZString* GetName(void);

	cIGZString* GetDescription(void);

	virtual uint32_t GetYearFirstAvailable(void);

	virtual SC4Percentage GetChanceAvailability(void);

	int64_t GetEnactmentIncome(void);

	int64_t GetRetracmentIncome(void);

	int64_t GetMonthlyConstantIncome(void);

	float GetMonthlyIncomeFactor(void);

	cISCPropertyHolder* GetMiscProperties();

	uint32_t GetAdvisorID(void);

	bool IsAvailable(void);

	bool IsOn(void);

	bool IsEnabled(void);

	virtual int64_t GetMonthlyAdjustedIncome(void);

	virtual bool CheckConditions(void);

	bool IsIncomeOrdinance(void);

	virtual bool Simulate(void);

	bool SetAvailable(bool isAvailable);
	
	bool SetOn(bool isOn);

	bool SetEnabled(bool isEnabled);

	bool ForceAvailable(bool isAvailable);

	bool ForceOn(bool isOn);

	bool ForceEnabled(bool isEnabled);

	bool ForceMonthlyAdjustedIncome(int64_t monthlyAdjustedIncome);

	virtual bool PostCityInit(cISC4City* pCity);

	virtual bool PreCityShutdown(cISC4City* pCity);

private:

	bool Write(cIGZOStream& stream);
	bool Read(cIGZIStream& stream);
	uint32_t GetGZCLSID();

	Logger& logger;

	uint32_t clsid;
	uint32_t refCount;
	cRZBaseString name;
	cRZBaseString description;
	int64_t enactmentIncome;
	int64_t retracmentIncome;
	int64_t monthlyConstantIncome;
	int64_t monthlyAdjustedIncome;
	float monthlyIncomeFactor;
	bool isIncomeOrdinance;
	cISC4ResidentialSimulator* pResidentialSimulator;
	cISC4Simulator* pSimulator;
	OrdinancePropertyHolder miscProperties;
	bool initialized;
	bool available;
	bool on;
	bool enabled;
	bool haveDeserialized;
};

