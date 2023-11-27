#pragma once
#include "cIGZUnknown.h"

class cIGZString;
class cISCPropertyHolder;
class SC4Percentage;

class cISC4Ordinance : public cIGZUnknown
{
	public:
		virtual bool Init(void) = 0;
		virtual bool Shutdown(void) = 0;

		virtual int32_t GetCurrentMonthlyIncome(void) = 0;

		virtual uint32_t GetID(void) = 0;
		virtual cIGZString* GetName(void) = 0;
		virtual cIGZString* GetDescription(void) = 0;

		virtual uint32_t GetYearFirstAvailable(void) = 0;
		virtual SC4Percentage GetChanceAvailability(void) = 0;

		virtual int32_t GetEnactmentIncome(void) = 0;
		virtual int32_t GetRetracmentIncome(void) = 0;
		virtual int32_t GetMonthlyConstantIncome(void) = 0;
		virtual float GetMonthlyIncomeFactor(void) = 0;

		virtual uint32_t GetAdvisorID(void) = 0;

		virtual cISCPropertyHolder* GetMiscProperties(void) = 0;

		virtual bool IsAvailable(void) = 0;
		virtual bool IsOn(void) = 0;
		virtual bool IsEnabled(void) = 0;

		// TODO: Is int32_t the correct return type?
		virtual int32_t GetMonthlyAdjustedIncome(void) = 0;

		virtual bool CheckConditions(void) = 0;

		virtual bool IsIncomeOrdinance(void) = 0;

		virtual bool Simulate(void) = 0;

		virtual bool SetAvailable(bool isAvailable) = 0;
		virtual bool SetOn(bool isOn) = 0;
		virtual bool SetEnabled(bool isEnabled) = 0;

		virtual bool ForceAvailable(bool isAvailable) = 0;
		virtual bool ForceOn(bool isAvailable) = 0;
		virtual bool ForceEnabled(bool isAvailable) = 0;

		// TODO: Is int32_t the correct parameter type?
		virtual bool ForceMonthlyAdjustedIncome(int32_t monthlyAdjustedIncome) = 0;
};