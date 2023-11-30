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
	// These values are copied from the Power Conservation ordinance
	static constexpr int64_t enactmentIncome = -20;
	static constexpr int64_t retracmentIncome = -10;
	static constexpr int64_t monthlyConstantIncome = 0;
	static constexpr float monthlyIncomeFactor = -0.005f;
	static constexpr bool isIncomeOrdinance = false;

public:

	TestOrdinance(uint32_t clsid, const char* name, const char* description)
		: OrdinanceBase(
			clsid,
			name,
			description,
			enactmentIncome,
			retracmentIncome,
			monthlyConstantIncome,
			monthlyIncomeFactor,
			isIncomeOrdinance)
	{
	}
};