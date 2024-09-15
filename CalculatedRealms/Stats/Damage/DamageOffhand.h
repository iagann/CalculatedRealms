#pragma once

#include <vector>
#include <string>
#include <map>

class DamageOffhand {
public:
	enum OFFHAND_TYPE {
		OFFHAND_TYPE_UNKNOWN,
		OFFHAND_TYPE_BURNING_SHIELD, OFFHAND_TYPE_ELECTRIC_DRAGONS, OFFHAND_TYPE_SPINNING_BLADE,
		OFFHAND_TYPE_DELUSION_OF_ZELKORS, OFFHAND_TYPE_CHAIN_LIGHTNING, OFFHAND_TYPE_ARCANE_APOCALYPSE,
		OFFHAND_TYPE_DRAGON_FLAME, OFFHAND_TYPE_EYE_OF_THE_STORM, OFFHAND_TYPE_ARCANE_ORB,
		OFFHAND_TYPE_RAIN_OF_FLAMES, OFFHAND_TYPE_STATIC_CHARGE, OFFHAND_TYPE_VORTEX,
		OFFHAND_TYPE_STARBLADES, OFFHAND_TYPE_TOXICITY,
		OFFHAND_TYPE_FIRE_BEAM, OFFHAND_TYPE_SPARK, OFFHAND_TYPE_DEATH_BLADE,
		OFFHAND_TYPE_CARNAGE_OF_FIRE, OFFHAND_TYPE_FEROCITY_OF_WOLVES, OFFHAND_TYPE_KRAKEN_SEED,
		OFFHAND_TYPE_FIRETY_TOTEM, OFFHAND_TYPE_LIGHTNING_PLASMA, OFFHAND_TYPE_ARCANE_TOTEM,
		OFFHAND_TYPE_FIRE_ORB, OFFHAND_TYPE_LIGHTNING_TOTEM, OFFHAND_TYPE_BLOOD_DRAGONS,
	};

	static const std::map<OFFHAND_TYPE, std::string>& getOffhandMap() {
		static const std::map<OFFHAND_TYPE, std::string> offhandMap = {
			{ OFFHAND_TYPE_UNKNOWN, "UNknown Offhand Type" },
			{ OFFHAND_TYPE_BURNING_SHIELD, "Burning shield" }, { OFFHAND_TYPE_ELECTRIC_DRAGONS, "Electric dragons" }, {OFFHAND_TYPE_SPINNING_BLADE, "Spinning blade" },
			{ OFFHAND_TYPE_DELUSION_OF_ZELKORS, "Delusion of Zelkors" }, { OFFHAND_TYPE_CHAIN_LIGHTNING, "Chain lightning" }, {OFFHAND_TYPE_ARCANE_APOCALYPSE, "Arcane apocalypse" },
			{ OFFHAND_TYPE_DRAGON_FLAME, "Dragon flame" }, { OFFHAND_TYPE_EYE_OF_THE_STORM, "Eye of the storm" }, { OFFHAND_TYPE_ARCANE_ORB, "Arcane orb" },
			{ OFFHAND_TYPE_RAIN_OF_FLAMES, "Rain of flames" }, {OFFHAND_TYPE_STATIC_CHARGE, "Static charge" }, { OFFHAND_TYPE_VORTEX, "Vortex" },
			{ OFFHAND_TYPE_STARBLADES, "Starblades" }, { OFFHAND_TYPE_TOXICITY, "Toxicity"},
			{ OFFHAND_TYPE_FIRE_BEAM, "Fire beam" }, { OFFHAND_TYPE_SPARK, "Spark" }, { OFFHAND_TYPE_DEATH_BLADE, "Death blade" },
			{ OFFHAND_TYPE_CARNAGE_OF_FIRE, "Carnage of Fire" }, { OFFHAND_TYPE_FEROCITY_OF_WOLVES, "Ferocity of Wolves" }, { OFFHAND_TYPE_KRAKEN_SEED, "Kraken Seed{"},
			{ OFFHAND_TYPE_FIRETY_TOTEM, "Fiery totem" }, { OFFHAND_TYPE_LIGHTNING_PLASMA, "Lightning plasma" }, { OFFHAND_TYPE_ARCANE_TOTEM, "Arcane totem" },
			{ OFFHAND_TYPE_FIRE_ORB, "Fire orb"}, { OFFHAND_TYPE_LIGHTNING_TOTEM, "Lightning totem"}, { OFFHAND_TYPE_BLOOD_DRAGONS, "Blood dragons"},
		};
		return offhandMap;
	}

	OFFHAND_TYPE type = OFFHAND_TYPE_UNKNOWN;
	std::map<OFFHAND_TYPE, double> damage;
	std::map<OFFHAND_TYPE, double> bonusDamage;

	DamageOffhand() {
		for (const auto& offhand : getOffhandMap()) {
			damage[offhand.first] = 0;
			bonusDamage[offhand.first] = 0;
		}
	}

	DamageOffhand operator+(const DamageOffhand& other) const {
		DamageOffhand result = *this;
		for (const auto& offhand : getOffhandMap()) {
			result.damage[offhand.first] += other.damage.at(offhand.first);
			result.bonusDamage[offhand.first] += other.bonusDamage.at(offhand.first);
		}
		result.type = other.type != OFFHAND_TYPE_UNKNOWN ? other.type : result.type;
		return result;
	}
	DamageOffhand& operator+=(const DamageOffhand& other) {
		*this = *this + other;
		return *this;
	}
	bool operator==(const DamageOffhand& other) const {
		return this->type == other.type
			&& this->damage == other.damage
			&& this->bonusDamage == other.bonusDamage;
	}
};