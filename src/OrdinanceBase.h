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
#include "cRZBaseString.h"
#include "OrdinancePropertyHolder.h"

class OrdinanceBase : public cISC4Ordinance
{
public:

	OrdinanceBase(
		uint32_t clsid,
		const char* name,
		const char* description);

	OrdinanceBase(
		uint32_t clsid,
		const char* name,
		const char* description,
		const OrdinancePropertyHolder& properties);

	OrdinanceBase(const OrdinanceBase& other) = delete;
	OrdinanceBase(OrdinanceBase&& other) noexcept = delete;

	bool QueryInterface(uint32_t riid, void** ppvObj);
	
	uint32_t AddRef();

	uint32_t Release();

	virtual bool Init(void);

	virtual bool Shutdown(void);

	virtual int32_t GetCurrentMonthlyIncome(void);

	uint32_t GetID(void);
	
	cIGZString* GetName(void);

	cIGZString* GetDescription(void);

	virtual uint32_t GetYearFirstAvailable(void);

	virtual SC4Percentage GetChanceAvailability(void);

	virtual cISCPropertyHolder* GetMiscProperties();

	virtual uint32_t GetAdvisorID(void);

	bool IsAvailable(void);

	bool IsOn(void);

	bool IsEnabled(void);

	virtual int32_t GetMonthlyAdjustedIncome(void);

	virtual bool CheckConditions(void);

	virtual bool Simulate(void);

	bool SetAvailable(bool isAvailable);
	
	bool SetOn(bool isOn);

	bool SetEnabled(bool isEnabled);

	bool ForceAvailable(bool isAvailable);

	bool ForceOn(bool isOn);

	bool ForceEnabled(bool isEnabled);

	bool ForceMonthlyAdjustedIncome(int32_t monthlyAdjustedIncome);

	virtual bool PostCityInit(cISC4City* pCity);

	virtual bool PreCityShutdown(cISC4City* pCity);

private:

	const uint32_t clsid;
	uint32_t refCount;
	cRZBaseString name;
	cRZBaseString description;
	uint32_t monthlyAdjustedIncome;
	cISC4ResidentialSimulator* pResidentialSimulator;
	cISC4Simulator* pSimulator;
	OrdinancePropertyHolder miscProperties;
	bool available;
	bool on;
	bool enabled;
};

