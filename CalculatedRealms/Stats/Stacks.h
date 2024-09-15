#pragma once

#include <map>
#include <string>

class Stacks {
public:
	enum STACK_TYPE {
		STACK_TYPE_AURA_DAMAGE_BUFF,
		STACK_TYPE_WARSTRIKE,
		STACK_TYPE_DEFIANCE,
		STACK_TYPE_COLLOSAL_FORCE,
		STACK_TYPE_ENVIGORATING_GUST,
		STACK_TYPE_PHANTOM_STRIKE,
		STACK_TYPE_TOLERANCE,
		STACK_TYPE_GRAND_VITALITY,
		STACK_TYPE_SLICE_N_DICE,
		STACK_TYPE_INFINITE_WARD,
		STACK_TYPE_HIGHTENED_SENCES,
		STACK_TYPE_MONSTER_HUNTER,
		STACK_TYPE_CARRIER_HAS_ARRIVED,
		STACK_TYPE_CLARITY,
		STACK_TYPE_BEAST_MODE,
		STACK_TYPE_SOUL_OF_THE_PROPHET,
		STACK_TYPE_THE_ELEMENTALIST,
		STACK_TYPE_THE_STRENGTH_AND_HONOR,
		STACK_TYPE_THE_ARCANE_FURY,
		STACK_TYPE_TASTE_MY_LIGHTNING,
		STACK_TYPE_FIRE_STARTER,
		STACK_TYPE_ANCESTRAL_BARRIER,
	};
	static const std::map<STACK_TYPE, std::string>& getStacksMap() {
		static const std::map<STACK_TYPE, std::string> stacksMap = {
			{ STACK_TYPE_AURA_DAMAGE_BUFF, "Aura Damage Buff" },
			{ STACK_TYPE_WARSTRIKE, "Warstrike" },
			{ STACK_TYPE_DEFIANCE, "Defiance" },
			{ STACK_TYPE_COLLOSAL_FORCE, "Colossal Force" },
			{ STACK_TYPE_ENVIGORATING_GUST, "Envigorating Gust" },
			{ STACK_TYPE_PHANTOM_STRIKE, "Phantom Strike" },
			{ STACK_TYPE_TOLERANCE, "Tolerance" },
			{ STACK_TYPE_GRAND_VITALITY, "Grand Vitality" },
			{ STACK_TYPE_SLICE_N_DICE, "Slice 'n Dice" },
			{ STACK_TYPE_INFINITE_WARD, "Infinite Ward" },
			{ STACK_TYPE_HIGHTENED_SENCES, "Hightened Sences" },
			{ STACK_TYPE_MONSTER_HUNTER, "Monster Hunter" },
			{ STACK_TYPE_CARRIER_HAS_ARRIVED, "Carrier has Arrived" },
			{ STACK_TYPE_CLARITY, "Clarity" },
			{ STACK_TYPE_BEAST_MODE, "Beast Mode" },
			{ STACK_TYPE_SOUL_OF_THE_PROPHET, "Soul of the Prophet" },
			{ STACK_TYPE_THE_ELEMENTALIST, "The Elementalist" },
			{ STACK_TYPE_THE_STRENGTH_AND_HONOR, "Strength and Honor" },
			{ STACK_TYPE_THE_ARCANE_FURY, "Arcane Fury" },
			{ STACK_TYPE_TASTE_MY_LIGHTNING, "Taste my Lightning" },
			{ STACK_TYPE_FIRE_STARTER, "Fire Starter" },
			{ STACK_TYPE_ANCESTRAL_BARRIER, "Ancestral Barrier" },
		};
		return stacksMap;
	}
	static const std::map<STACK_TYPE, int>& getMaxStacksMap() {
		static const std::map<STACK_TYPE, int> maxStacksMap = {
			{ STACK_TYPE_AURA_DAMAGE_BUFF, 5 },
			{ STACK_TYPE_WARSTRIKE, 1 },
			{ STACK_TYPE_DEFIANCE, 1 },
			{ STACK_TYPE_COLLOSAL_FORCE, 1 },
			{ STACK_TYPE_ENVIGORATING_GUST, 1 },
			{ STACK_TYPE_PHANTOM_STRIKE, 1 },
			{ STACK_TYPE_TOLERANCE, 1 },
			{ STACK_TYPE_GRAND_VITALITY, 1 },
			{ STACK_TYPE_SLICE_N_DICE, 1 },
			{ STACK_TYPE_INFINITE_WARD, 1 },
			{ STACK_TYPE_HIGHTENED_SENCES, 1 },
			{ STACK_TYPE_MONSTER_HUNTER, 1 },
			{ STACK_TYPE_CARRIER_HAS_ARRIVED, 1 },
			{ STACK_TYPE_CLARITY, 1 },
			{ STACK_TYPE_BEAST_MODE, 1 },
			{ STACK_TYPE_SOUL_OF_THE_PROPHET, 1 },
			{ STACK_TYPE_THE_ELEMENTALIST, 1 },
			{ STACK_TYPE_THE_STRENGTH_AND_HONOR, 1 },
			{ STACK_TYPE_THE_ARCANE_FURY, 100 },
			{ STACK_TYPE_TASTE_MY_LIGHTNING, 100 },
			{ STACK_TYPE_FIRE_STARTER, 100 },
			{ STACK_TYPE_ANCESTRAL_BARRIER, 1 },
		};
		return maxStacksMap;
	}
	static const std::map<STACK_TYPE, int>& getRecommendedStacksMap() {
		static const std::map<STACK_TYPE, int> recommendedStacksMap = {
			{ STACK_TYPE_AURA_DAMAGE_BUFF, 5 },
			{ STACK_TYPE_WARSTRIKE, 1 },
			{ STACK_TYPE_DEFIANCE, 1 },
			{ STACK_TYPE_COLLOSAL_FORCE, 1 },
			{ STACK_TYPE_ENVIGORATING_GUST, 1 },
			{ STACK_TYPE_PHANTOM_STRIKE, 1 },
			{ STACK_TYPE_TOLERANCE, 1 },
			{ STACK_TYPE_GRAND_VITALITY, 1 },
			{ STACK_TYPE_SLICE_N_DICE, 1 },
			{ STACK_TYPE_INFINITE_WARD, 1 },
			{ STACK_TYPE_HIGHTENED_SENCES, 1 },
			{ STACK_TYPE_MONSTER_HUNTER, 1 },
			{ STACK_TYPE_CARRIER_HAS_ARRIVED, 1 },
			{ STACK_TYPE_CLARITY, 1 },
			{ STACK_TYPE_BEAST_MODE, 1 },
			{ STACK_TYPE_SOUL_OF_THE_PROPHET, 1 },
			{ STACK_TYPE_THE_ELEMENTALIST, 1 },
			{ STACK_TYPE_THE_STRENGTH_AND_HONOR, 1 },
			{ STACK_TYPE_THE_ARCANE_FURY, 30 },
			{ STACK_TYPE_TASTE_MY_LIGHTNING, 30 },
			{ STACK_TYPE_FIRE_STARTER, 30 },
			{ STACK_TYPE_ANCESTRAL_BARRIER, 1 },
		};
		return recommendedStacksMap;
	}

private:
	std::map<STACK_TYPE, int> current;

public:
	void increase(const STACK_TYPE& stack, const int& value = 1) {
		current[stack] = std::min(current[stack] + value, getRecommendedStacksMap().at(stack));
	}
	int getCurrent(const STACK_TYPE& stack) const {
		return current.at(stack);
	}

	Stacks() {
		for (const auto& stack : getStacksMap()) {
			current.insert(std::make_pair(stack.first, 0));
		}
	}
	static const int AURA_DAMAGE_BUFF = 5;

	Stacks operator+(const Stacks& other) const {
		Stacks result = *this;
		for (const auto& maxStack : getMaxStacksMap()) {
			result.current[maxStack.first] += std::min(other.current.at(maxStack.first), maxStack.second);
		}
		return result;
	}
	Stacks& operator+=(const Stacks& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const Stacks& other) const {
		for (const auto& stack : current) {
			if (stack.second != other.current.at(stack.first))
				return false;
		}
		return true;
	}
};