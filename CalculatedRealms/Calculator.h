#pragma once

#include "Stats/Stats.h"

#include <string>
#include <list>

class Calculator {
private:
	Stats init;
	std::map<std::string, std::vector<Stats>> stats;
	Stats cards;
	Stats types;
	Stats prediction;

	Stats total;
	AttributesBase totalAttributes;

	std::list<std::map<std::string, int>> getAllStatsCombinations();
public:
	int verbose = 0;

	void setInit(const Stats& s) { init = s; }
	void setStats(const std::map<std::string, std::vector<Stats>>& s) { stats = s; }
	void setCards(const Stats& s) { cards = s; }
	void setPrediction(const Stats& s) { prediction = s; }
	void setWeaponType(const DamageWeapon::WEAPON_TYPE& type) { types.damage.weapon.type = type; }
	void setOffhandName(const DamageOffhand::OFFHAND_TYPE& type) { types.damage.offhand.type = type; }
	std::pair<double, double> getRating() { return getRating(getBestItemCombo()); }
	std::pair<double, double> getRating(const std::map<std::string, int>& combo);
	double getRatingSimple() { auto r = getRating(); return r.first * r.second; }
	double getRatingSimple(const std::map<std::string, int>& combo) { auto r = getRating(combo); return r.first * r.second; }
	const std::map<std::string, int> getBestItemCombo(bool reset = false);

	Calculator copy();
};