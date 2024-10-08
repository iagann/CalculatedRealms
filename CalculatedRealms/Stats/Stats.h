#pragma once

#include "Attributes.h"
#include "Damage.h"
#include "Survivability.h"
#include "Per.h"
#include "Stacks.h"

class Stats {
public:
	Attributes attributes;
	Damage damage;
	Survivability survivability;
	Per per;
	Stacks stacks;
	
	double xpBonus = 0;
	double extraInventorySlots = 0;
	double potionSlots = 0;
	double characterLevel = 0;
	double rupture = 0;

	Stats operator+(const Stats& other) const {
		Stats result = *this;

		result.attributes += other.attributes;
		result.damage += other.damage;
		result.survivability += other.survivability;
		result.per += other.per;
		result.stacks += other.stacks;
		result.xpBonus += other.xpBonus;
		result.extraInventorySlots += other.extraInventorySlots;
		result.potionSlots += other.potionSlots;

		if (result.characterLevel < other.characterLevel)
			result.characterLevel = other.characterLevel;
		if (result.rupture < other.rupture)
			result.rupture = other.rupture;

		return result;
	}

	Stats& operator+=(const Stats& other) {
		*this = *this + other;
		return *this;
	}

	bool operator==(const Stats& other) const {
		return this->attributes == other.attributes
			&& this->damage == other.damage
			&& this->survivability == other.survivability
			&& this->per == other.per
			&& this->stacks == other.stacks
			&& this->xpBonus == other.xpBonus
			&& this->extraInventorySlots == other.extraInventorySlots
			&& this->characterLevel == other.characterLevel
			&& this->rupture == other.rupture;
	}
	bool operator!=(const Stats& other) const {
		return !(*this == other);
	}

	static Stats init() {
		Stats result;

		result.attributes.bonus.strength = 1;
		result.attributes.bonus.agility = 1;
		result.attributes.bonus.stamina = 1;
		result.attributes.bonus.endurance = 1;
		result.attributes.bonus.luck = 1;
		result.attributes.bonus.dexterity = 1;
		result.attributes.bonus.wisdom = 1;

		result.damage.base.flat = 5;
		result.damage.base.critChance = 0.01;
		result.damage.base.critDamage = 1.1;
		result.damage.base.attackSpeed = 110;
		result.damage.base.increased = 1;
		result.damage.base.bossDamage = 1;

		result.damage.elemental.arcane = 1;
		result.damage.elemental.fire = 1;
		result.damage.elemental.lightning = 1;

		for (const auto& offhand : DamageOffhand::getOffhandMap()) {
			result.damage.offhand.damage[offhand.first] = 1;
			result.damage.offhand.bonusDamage[offhand.first] = 1;
		}

		result.survivability.armorBonus = 1;
		result.survivability.maxHealth = 100;
		result.survivability.healthBonus = 1;
		result.survivability.healthRegen = 1;
		result.survivability.hpRegenBonus = 0;
		result.survivability.maxEnergy = 100;
		result.survivability.energyRegen = 1.2;
		result.survivability.energyRegenBonus = 1;

		result.potionSlots = 3;

		return result;
	}
};