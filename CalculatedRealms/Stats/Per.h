#pragma once

class Per {
public:
	double increasedDamagePerDex = 0;
	double critChancePerDex = 0;
	double damagePerExtraInventorySlot = 0;
	double damagePerEndurance = 0;
	double attackSpeedPerPotionSlot = 0;
	double critDamagePerArmour = 0;

	Per operator+(const Per& other) const {
		Per result = *this;
		result.increasedDamagePerDex += other.increasedDamagePerDex;
		result.critChancePerDex += other.critChancePerDex;
		result.damagePerExtraInventorySlot += other.damagePerExtraInventorySlot;
		result.damagePerEndurance += other.damagePerEndurance;
		result.attackSpeedPerPotionSlot += other.attackSpeedPerPotionSlot;
		result.critDamagePerArmour += other.critDamagePerArmour;
		return result;
	}
	Per& operator+=(const Per& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const Per& other) const {
		return this->increasedDamagePerDex == other.increasedDamagePerDex
			&& this->critChancePerDex == other.critChancePerDex
			&& this->damagePerExtraInventorySlot == other.damagePerExtraInventorySlot
			&& this->damagePerEndurance == other.damagePerEndurance
			&& this->attackSpeedPerPotionSlot == other.attackSpeedPerPotionSlot
			&& this->critDamagePerArmour == other.critDamagePerArmour;
	}
};