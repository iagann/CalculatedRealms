#pragma once

class DamageElemental {
public:
	enum ELEMENT_TYPE {
		ELEMENT_TYPE_UNDEFINED,
		ELEMENT_TYPE_FIRE,
		ELEMENT_TYPE_LIGHTNING,
		ELEMENT_TYPE_ARCANE
	};

	double fire = 0;
	double lightning = 0;
	double arcane = 0;

	ELEMENT_TYPE mainType = ELEMENT_TYPE_UNDEFINED;
	ELEMENT_TYPE secondaryType = ELEMENT_TYPE_UNDEFINED;

	DamageElemental operator+(const DamageElemental& other) const {
		DamageElemental result = *this;
		result.fire += other.fire;
		result.lightning += other.lightning;
		result.arcane += other.arcane;
		result.mainType = (this->mainType == ELEMENT_TYPE_UNDEFINED) ? other.mainType : this->mainType;
		result.secondaryType = (this->secondaryType == ELEMENT_TYPE_UNDEFINED) ? other.secondaryType : this->secondaryType;
		return result;
	}
	DamageElemental& operator+=(const DamageElemental& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const DamageElemental& other) const {
		return this->fire == other.fire
			&& this->lightning == other.lightning
			&& this->arcane == other.arcane
			&& this->mainType == other.mainType
			&& this->secondaryType == other.secondaryType;
	}

	double getMultiplier(const Stacks& stacks) const { return getValue(mainType, stacks) + getValue(secondaryType, stacks) - 1; }
private:
	double getValue(const ELEMENT_TYPE& type, const Stacks& stacks) const {
		double additional = stacks.getCurrent(Stacks::STACK_TYPE_DEFIANCE) * 0.1
			+ stacks.getCurrent(Stacks::STACK_TYPE_THE_ELEMENTALIST) * 0.5;
		switch (type) {
		case ELEMENT_TYPE_UNDEFINED: return 1;
		case ELEMENT_TYPE_FIRE: return fire + additional + stacks.getCurrent(Stacks::STACK_TYPE_FIRE_STARTER) * 0.01;
		case ELEMENT_TYPE_LIGHTNING: return lightning + additional + stacks.getCurrent(Stacks::STACK_TYPE_TASTE_MY_LIGHTNING) * 0.01;
		case ELEMENT_TYPE_ARCANE: return arcane + additional + stacks.getCurrent(Stacks::STACK_TYPE_THE_ARCANE_FURY) * 0.01;
		}
	}
};