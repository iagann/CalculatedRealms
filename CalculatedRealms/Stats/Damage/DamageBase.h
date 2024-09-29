#pragma once

class DamageBase {
public:
	double flat = 0;
	double critChance = 0;
	double critDamage = 0;
	double attackSpeed = 0;
	double increased = 0;
	double bossDamage = 0;

	bool doubleDamage = false;

	DamageBase operator+(const DamageBase& other) const {
		DamageBase result = *this;
		result.flat += other.flat;
		result.critChance += other.critChance;
		result.critDamage += other.critDamage;
		result.attackSpeed += other.attackSpeed;
		result.increased += other.increased;
		result.bossDamage += other.bossDamage;

		result.doubleDamage = doubleDamage || other.doubleDamage;

		return result;
	}
	DamageBase& operator+=(const DamageBase& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const DamageBase& other) const {
		return this->flat == other.flat
			&& this->critChance == other.critChance
			&& this->critDamage == other.critDamage
			&& this->attackSpeed == other.attackSpeed
			&& this->increased == other.increased
			&& this->bossDamage == other.bossDamage
			&& this->doubleDamage == other.doubleDamage;
	}
};