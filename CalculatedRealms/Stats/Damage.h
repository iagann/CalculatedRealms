#pragma once

#include "Damage/DamageBase.h"
#include "Damage/DamageElemental.h"
#include "Damage/DamageOffhand.h"
#include "Damage/DamageWeapon.h"

class Damage {
public:
	DamageBase base;
	DamageElemental elemental;
	DamageOffhand offhand;
	DamageWeapon weapon;

	Damage operator+(const Damage& other) const {
		Damage result = *this;
		result.base += other.base;
		result.elemental += other.elemental;
		result.offhand += other.offhand;
		result.weapon += other.weapon;
		return result;
	}
	Damage& operator+=(const Damage& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const Damage& other) const {
		return this->base == other.base
			&& this->elemental == other.elemental
			&& this->offhand == other.offhand
			&& this->weapon == other.weapon;
	}
};