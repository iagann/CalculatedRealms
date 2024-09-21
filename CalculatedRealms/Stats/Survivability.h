#pragma once

class Survivability {
public:
	double armor = 0;
	double armorBonus = 0;
	double maxHealth = 0;
	double healthBonus = 0;
	double healthRegen = 0;
	double hpRegenBonus = 0;
	double maxEnergy = 0;
	double energyRegen = 0;
	double energyRegenBonus = 0;
	double lifeSteal = 0;
	double lifeStealChance = 0;
	double damageReduction = 0;
	bool disableRegen = false;
	bool disableLeech = false;

	Survivability operator+(const Survivability& other) const {
		Survivability result = *this;
		result.armor += other.armor;
		result.armorBonus += other.armorBonus;
		result.maxHealth += other.maxHealth;
		result.healthBonus += other.healthBonus;
		result.healthRegen += other.healthRegen;
		result.hpRegenBonus += other.hpRegenBonus;
		result.maxEnergy += other.maxEnergy;
		result.energyRegen += other.energyRegen;
		result.energyRegenBonus += other.energyRegenBonus;
		result.lifeSteal += other.lifeSteal;
		result.lifeStealChance += other.lifeStealChance;
		result.damageReduction += other.damageReduction;
		result.disableRegen = disableRegen || other.disableRegen;
		result.disableLeech = disableLeech || other.disableLeech;
		return result;
	}

	Survivability& operator+=(const Survivability& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const Survivability& other) const {
		return this->armor == other.armor
			&& this->armorBonus == other.armorBonus
			&& this->maxHealth == other.maxHealth
			&& this->healthBonus == other.healthBonus
			&& this->healthRegen == other.healthRegen
			&& this->hpRegenBonus == other.hpRegenBonus
			&& this->maxEnergy == other.maxEnergy
			&& this->energyRegen == other.energyRegen
			&& this->energyRegenBonus == other.energyRegenBonus
			&& this->lifeSteal == other.lifeSteal
			&& this->lifeStealChance == other.lifeStealChance
			&& this->damageReduction == other.damageReduction
			&& this->disableRegen == other.disableRegen
			&& this->disableLeech == other.disableLeech;
	}
};