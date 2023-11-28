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
#include "OrdinanceBase.h"

class TestOrdinance final : public OrdinanceBase
{
public:
	TestOrdinance(uint32_t clsid, const char* name, const char* description)
		: OrdinanceBase(clsid, name, description)
	{
	}

	// These values are copied from the Power Conservation ordinance
	int32_t GetEnactmentIncome()
	{
		return -20;
	}

	int32_t GetRetracmentIncome()
	{
		return -10;
	}

	int32_t GetMonthlyConstantIncome()
	{
		return 0;
	}

	float GetMonthlyIncomeFactor()
	{
		return -0.005f;
	}

	bool IsIncomeOrdinance()
	{
		return false;
	}
};