#pragma once

#include <map>
#include <string>

class DamageWeaponBase {
public:
	double damage = 0;
	double criticalDamage = 0;
	double criticalChance = 0;

	DamageWeaponBase operator+(const DamageWeaponBase& other) const {
		DamageWeaponBase result = *this;
		result.damage += other.damage;
		result.criticalDamage += other.criticalDamage;
		result.criticalChance += other.criticalChance;
		return result;
	}
	DamageWeaponBase& operator+=(const DamageWeaponBase& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const DamageWeaponBase& other) const {
		return this->damage == other.damage
			&& this->criticalDamage == other.criticalDamage
			&& this->criticalChance == other.criticalChance;
	}
};

class DamageWeapon {
public:
	enum WEAPON_TYPE {
		WEAPON_TYPE_UNARMED,
		WEAPON_TYPE_MAGERY,
		WEAPON_TYPE_SPEAR,
		WEAPON_TYPE_MAULS,
		WEAPON_TYPE_STAVE,
		WEAPON_TYPE_TWO_HANDED,
		WEAPON_TYPE_ONE_HANDED,
		WEAPON_TYPE_ARCHERY,
	};

	static const std::map<WEAPON_TYPE, std::string>& getWeaponMap() {
		static const std::map<WEAPON_TYPE, std::string> offhandMap = {
			{ WEAPON_TYPE_UNARMED, "Unarmed" },
			{ WEAPON_TYPE_MAGERY, "Magery" },
			{ WEAPON_TYPE_SPEAR, "Spear" },
			{ WEAPON_TYPE_MAULS, "Maul" },
			{ WEAPON_TYPE_STAVE, "Stave" },
			{ WEAPON_TYPE_TWO_HANDED, "Two handed" },
			{ WEAPON_TYPE_ONE_HANDED, "One handed" },
			{ WEAPON_TYPE_ARCHERY, "Archery" },
		};
		return offhandMap;
	}

	WEAPON_TYPE type = WEAPON_TYPE_UNARMED;
	std::map<WEAPON_TYPE, double> damage;
	std::map<WEAPON_TYPE, double> criticalDamage;
	std::map<WEAPON_TYPE, double> criticalChance;

	DamageWeapon() {
		for (const auto& weapon : getWeaponMap()) {
			damage[weapon.first] = 0;
			criticalDamage[weapon.first] = 0;
			criticalChance[weapon.first] = 0;
		}
	}

	DamageWeapon operator+(const DamageWeapon& other) const {
		DamageWeapon result = *this;
		for (const auto& weapon : DamageWeapon::getWeaponMap()) {
			result.damage[weapon.first] += other.damage.at(weapon.first);
			result.criticalDamage[weapon.first] += other.criticalDamage.at(weapon.first);
			result.criticalChance[weapon.first] += other.criticalChance.at(weapon.first);
		}
		result.type = other.type != WEAPON_TYPE_UNARMED ? other.type : result.type;
		return result;
	}
	DamageWeapon& operator+=(const DamageWeapon& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const DamageWeapon& other) const {
		for (const auto& weapon : DamageWeapon::getWeaponMap()) {
			if (this->damage.at(weapon.first) != other.damage.at(weapon.first)
				|| this->criticalDamage.at(weapon.first) != other.criticalDamage.at(weapon.first)
				|| this->criticalChance.at(weapon.first) != other.criticalChance.at(weapon.first))
				return false;
		}
		return true;
	}
};