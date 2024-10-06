#pragma once

class Per {
public:
	double increasedDamagePerDex = 0;
	double critChancePerLuck = 0;
	double damagePerExtraInventorySlot = 0;
	double damagePerEndurance = 0;
	double attackSpeedPerPotionSlot = 0;
	double damagePerPotionSlot = 0;
	double armourPerPotionSlot = 0;
	double critDamagePerArmour = 0;
	double bossDamagePerLuck = 0;
	double damagePerLuck = 0;
	double bonusHealthPerLuck = 0;

	Per operator+(const Per& other) const {
		Per result = *this;
		result.increasedDamagePerDex += other.increasedDamagePerDex;
		result.critChancePerLuck += other.critChancePerLuck;
		result.damagePerExtraInventorySlot += other.damagePerExtraInventorySlot;
		result.damagePerEndurance += other.damagePerEndurance;
		result.attackSpeedPerPotionSlot += other.attackSpeedPerPotionSlot;
		result.damagePerPotionSlot += other.damagePerPotionSlot;
		result.armourPerPotionSlot += other.armourPerPotionSlot;
		result.critDamagePerArmour += other.critDamagePerArmour;
		result.bossDamagePerLuck += other.bossDamagePerLuck;
		result.damagePerLuck += other.damagePerLuck;
		result.bonusHealthPerLuck += other.bonusHealthPerLuck;
		return result;
	}
	Per& operator+=(const Per& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const Per& other) const {
		return this->increasedDamagePerDex == other.increasedDamagePerDex
			&& this->critChancePerLuck == other.critChancePerLuck
			&& this->damagePerExtraInventorySlot == other.damagePerExtraInventorySlot
			&& this->damagePerEndurance == other.damagePerEndurance
			&& this->attackSpeedPerPotionSlot == other.attackSpeedPerPotionSlot
			&& this->damagePerPotionSlot == other.damagePerPotionSlot
			&& this->armourPerPotionSlot == other.armourPerPotionSlot
			&& this->critDamagePerArmour == other.critDamagePerArmour
			&& this->bossDamagePerLuck == other.bossDamagePerLuck
			&& this->damagePerLuck == other.damagePerLuck
			&& this->bonusHealthPerLuck == other.bonusHealthPerLuck;
	}
};