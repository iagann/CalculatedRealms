#pragma once

#include "Stacks.h"

class AttributesBase {
public:
	double strength = 0;
	double agility = 0;
	double stamina = 0;
	double endurance = 0;
	double luck = 0;
	double dexterity = 0;
	double wisdom = 0;

	AttributesBase operator+(const AttributesBase& other) const {
		AttributesBase result = *this;
		result.strength += other.strength;
		result.agility += other.agility;
		result.stamina += other.stamina;
		result.endurance += other.endurance;
		result.luck += other.luck;
		result.dexterity += other.dexterity;
		result.wisdom += other.wisdom;
		return result;
	}
	AttributesBase operator*(const AttributesBase& other) const {
		AttributesBase result = *this;
		result.strength *= other.strength;
		result.agility *= other.agility;
		result.stamina *= other.stamina;
		result.endurance *= other.endurance;
		result.luck *= other.luck;
		result.dexterity *= other.dexterity;
		result.wisdom *= other.wisdom;
		return result;
	}
	AttributesBase& operator=(const AttributesBase& other) {
		this->strength = other.strength;
		this->agility = other.agility;
		this->stamina = other.stamina;
		this->endurance = other.endurance;
		this->luck = other.luck;
		this->dexterity = other.dexterity;
		this->wisdom = other.wisdom;
		return *this;
	}
	AttributesBase& operator+=(const AttributesBase& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const AttributesBase& other) const {
		return this->strength == other.strength
			&& this->agility == other.agility
			&& this->stamina == other.stamina
			&& this->endurance == other.endurance
			&& this->luck == other.luck
			&& this->dexterity == other.dexterity
			&& this->wisdom == other.wisdom;
	}
};

class Attributes {
public:
	AttributesBase base;
	AttributesBase bonus;

	Attributes operator+(const Attributes& other) const {
		Attributes result = *this;
		result.base += other.base;
		result.bonus += other.bonus;
		return result;
	}
	Attributes& operator=(const Attributes& other) {
		this->base = other.base;
		this->bonus = other.bonus;
		return *this;
	}
	Attributes& operator+=(const Attributes& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const Attributes& other) const {
		return this->base == other.base
			&& this->bonus == other.bonus;
	}

	AttributesBase total(const Stacks& stacks) const {
		AttributesBase resultBase = base;
		AttributesBase resultBonus = bonus;

		resultBase.strength += stacks.getCurrent(Stacks::STACK_TYPE_COLLOSAL_FORCE) * 50;

		resultBonus.strength += stacks.getCurrent(Stacks::STACK_TYPE_THE_STRENGTH_AND_HONOR) * 0.5;
		resultBonus.agility += stacks.getCurrent(Stacks::STACK_TYPE_SLICE_N_DICE) * 0.1;
		resultBonus.endurance += stacks.getCurrent(Stacks::STACK_TYPE_TOLERANCE) * 0.1;

		return resultBase * resultBonus;
	}
};