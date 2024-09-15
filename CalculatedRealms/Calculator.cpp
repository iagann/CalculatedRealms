#include "Calculator.h"
#include "Util.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip> // For std::setw and std::fixed
#include <algorithm> // For std::max
#include <set>
#include <numeric>  // For std::accumulate

//#define E 2.71828182845904523536
#define REGEN_WEIGHT 10

std::list<std::map<std::string, int>> Calculator::getAllStatsCombinations() {
	std::list<std::map<std::string, int>> result;
	std::list<std::map<std::string, int>> result2;

	result.push_back(std::map<std::string, int>());

	for (const auto& stat : stats) {
		for (const auto& r : result) {
			
			for (int i = 0; i < stat.second.size(); ++i) {
				auto c = r;
				c[stat.first] = i;
				result2.push_back(c);
			}
		}
		result.clear();
		result = result2;
		result2.clear();
	}

	return result;
}

const std::map<std::string, int> Calculator::getBestItemCombo() {
	static std::map<std::string, std::vector<Stats>> lastComputedStats;
	static std::map<std::string, int> lastBestCombo;
	if (lastComputedStats == stats) {
		return lastBestCombo;
	}

	/*std::cout << "Calculating best item set: " << std::endl;

	const auto& allCombos = getAllStatsCombinations();
	std::map<std::string, int> result;
	double maxRating = 0;
	for (const auto& combo : allCombos) {
		double rating = getRating(combo);
		if (rating > maxRating) {
			maxRating = rating;
			result = combo;
		}
	}
	return result;*/

	// Retrieve all possible combinations
	const auto& allCombos = getAllStatsCombinations();
	if (allCombos.size() == 1)
		return *allCombos.begin();

	std::cout << std::endl << "Calculating best item set:\n" << std::endl;

	
	std::map<std::string, int> bestCombo;
	std::pair<double, double> maxRating = {0, 0};
	std::vector<std::pair<std::map<std::string, int>, std::pair<double, double>>> ratings; // To store ratings of all combinations

	// Determine the best combination and store all ratings
	for (const auto& combo : allCombos) {
		std::pair<double, double> rating = getRating(combo);
		ratings.push_back(std::make_pair(combo, rating));
		if (rating.first * rating.second > maxRating.first * maxRating.second) {
			maxRating = rating;
			bestCombo = combo;
		}
	}

	std::sort(ratings.begin(), ratings.end(), [](const std::pair<std::map<std::string, int>, std::pair<double, double>>& a,
		const std::pair<std::map<std::string, int>, std::pair<double, double>>& b) -> bool {
			return a.second.first * a.second.second > b.second.first * b.second.second; // Descending order
		});

	// Collect all unique item keys from combinations
	std::set<std::string> items;
	for (const auto& combo : allCombos) {
		for (const auto& pair : combo) {
			if (items.count(pair.first))
				continue;
			if (pair.second != 0) {
				items.insert(pair.first);
				break;
			}
		}
	}

	// Prepare to print the table
	// Header
	const int width0 = 20;
	const int width = 6;
	std::cout << std::left << std::setw(width0) << "Item/Combo";
	for (size_t i = 0; i < allCombos.size(); ++i) {
		std::cout << std::left << std::setw(width) << (std::to_string(i + 1));
	}
	//std::cout << std::left << std::setw(width) << "Rating (%)" << std::endl;
	std::cout << std::endl;

	// Separator
	int totalColumns = 1 + allCombos.size() + 1; // Item + combinations + rating
	for (int i = 0; i < totalColumns * width + width0 - width; ++i) std::cout << "-";
	std::cout << std::endl;

	// Rows for each item
	for (const auto& item : items) {
		std::cout << std::left << std::setw(width0) << item.substr(0, width0 -1);
		for (const auto& rating : ratings) {
			auto it = rating.first.find(item);
			if (it != rating.first.end()) {
				std::cout << std::left << std::setw(width) << it->second;
			}
			else {
				std::cout << std::left << std::setw(width) << "N/A";
			}
		}
		// Empty space for Rating row
		//std::cout << std::left << std::setw(width) << "" << std::endl;
		std::cout << std::endl;
	}

	// Separator
	for (int i = 0; i < totalColumns * width + width0 - width; ++i) std::cout << "-";
	std::cout << std::endl;

	// Rating row
	std::cout << std::left << std::setw(width0) << "Dps% of max";
	for (const auto& rating : ratings) {
		double percentage = (maxRating.first > 0.0) ? (rating.second.first / maxRating.first) * 100.0 : 0.0;
		// Ensure that the percentage does not exceed 100%
		//percentage = std::min<double>(percentage, 100.0);
		std::cout << std::left << std::setw(width) << std::fixed << std::setprecision(1) << percentage;
	}
	// Empty space for the last column (Rating label)
	std::cout << std::left << std::setw(width) << "" << std::endl;

	// Rating row
	std::cout << std::left << std::setw(width0) << "EHP% of max";
	for (const auto& rating : ratings) {
		double percentage = (maxRating.second > 0.0) ? (rating.second.second / maxRating.second) * 100.0 : 0.0;
		// Ensure that the percentage does not exceed 100%
		//percentage = std::min<double>(percentage, 100.0);
		std::cout << std::left << std::setw(width) << std::fixed << std::setprecision(1) << percentage;
	}
	// Empty space for the last column (Rating label)
	std::cout << std::left << std::setw(width) << "" << std::endl;

	// Rating row
	std::cout << std::left << std::setw(width0) << "Rating% of max";
	for (const auto& rating : ratings) {
		double percentage = (maxRating.first * maxRating.second > 0.0) ? ((rating.second.first * rating.second.second) / (maxRating.first * maxRating.second)) * 100.0 : 0.0;
		// Ensure that the percentage does not exceed 100%
		//percentage = std::min<double>(percentage, 100.0);
		std::cout << std::left << std::setw(width) << std::fixed << std::setprecision(1) << percentage;
	}
	// Empty space for the last column (Rating label)
	std::cout << std::left << std::setw(width) << "" << std::endl;

	lastComputedStats = stats;
	lastBestCombo = bestCombo;

	return bestCombo;
}

std::pair<double, double> Calculator::getRating(const std::map<std::string, int>&combo) {
	Stats comboStats;
	for (const auto statId : combo) {
		comboStats += stats.at(statId.first).at(statId.second);
	}

	total = init + comboStats + cards + types + prediction;
	totalAttributes = total.attributes.total(total.stacks);

	if (verbose) {
		const auto secondColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		const auto thirdColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		if(verbose) { std::cout << std::endl << "Total attributes:" << std::endl; }
		Util::SetConsoleColor(secondColor);
		std::cout << "\tStrength: " << totalAttributes.strength << std::endl;
		std::cout << "\tStamina: " << totalAttributes.stamina << std::endl;
		std::cout << "\tDexterity: " << totalAttributes.dexterity << std::endl;
		std::cout << "\tAgility: " << totalAttributes.agility << std::endl;
		std::cout << "\tLuck: " << totalAttributes.luck << std::endl;
		std::cout << "\tEndurance: " << totalAttributes.endurance << std::endl;
		std::cout << "\tWisdomstrength: " << totalAttributes.wisdom << std::endl;
		Util::ResetConsoleColor();
	}
	/*
	Util::SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "Total rating: " << totalRating << std::endl;
	Util::ResetConsoleColor();
	*/
	return { getDamageRating(), getSurvivalRating() };
}

double Calculator::getDamageRating() {
	const auto secondColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	const auto thirdColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	if (verbose) { std::cout << std::endl << "Explaining damage:" << std::endl; }

	std::vector<double> intermediate = {
			total.damage.base.flat,
			totalAttributes.strength * 0.5,
			total.per.damagePerEndurance * (std::floor(totalAttributes.endurance / 150) * 150),
			total.stacks.getCurrent(Stacks::STACK_TYPE_WARSTRIKE) * 30.0,
			total.stacks.getCurrent(Stacks::STACK_TYPE_PHANTOM_STRIKE) * 20.0,
	};
	double totalFlat = std::accumulate(intermediate.begin(), intermediate.end(), 0.0);
	if (verbose) {
		Util::SetConsoleColor(thirdColor);
		std::cout << "\t      + Flat damage: " << intermediate[0] << std::endl;
		if (intermediate[1]) std::cout << "\t      + Flat damage from strength: " << intermediate[1] << std::endl;
		if (intermediate[2]) std::cout << "\t      + Flat damage from 'damage per endurance': " << intermediate[2] << std::endl;
		if (intermediate[3]) std::cout << "\t      + Flat damage from Warstrike tree node: " << intermediate[3] << std::endl;
		if (intermediate[4]) std::cout << "\t      + Flat damage from Phantom Strike tree node: " << intermediate[4] << std::endl;

		Util::SetConsoleColor(secondColor);
		std::cout << "\tTotal flat damage: " << totalFlat << std::endl;
	}

	intermediate = {
			total.damage.base.increased,
			total.per.increasedDamagePerDex * (std::floor(totalAttributes.dexterity / 10) * 10),
			total.per.damagePerExtraInventorySlot * total.extraInventorySlots
				+ total.stacks.getCurrent(Stacks::STACK_TYPE_CARRIER_HAS_ARRIVED) * 20 * total.per.damagePerExtraInventorySlot,
			total.stacks.getCurrent(Stacks::STACK_TYPE_THE_STRENGTH_AND_HONOR) * 0.05,
			total.stacks.getCurrent(Stacks::STACK_TYPE_FIRE_STARTER) * 0.01
	};
	double increased = std::accumulate(intermediate.begin(), intermediate.end(), 0.0);
	if (verbose) {
		Util::SetConsoleColor(thirdColor);
		std::cout << "\t      + Increased damage: " << intermediate[0] * 100 - 100 << "%" << std::endl;
		if (intermediate[1]) std::cout << "\t      + Increased damage from dexterity: " << intermediate[1] * 100 << "%" << std::endl;
		if (intermediate[2]) std::cout << "\t      + Increased damage from inventory slots: " << intermediate[2] * 100 << "%" << std::endl;
		if (intermediate[3]) std::cout << "\t      + Increased damage from Strength and Honor tree node: " << intermediate[3] * 100 << "%" << std::endl;
		if (intermediate[4]) std::cout << "\t      + Increased damage from The Fire Starter tree node: " << intermediate[4] * 100 << "%" << std::endl;
	}

	double increasedFlat = totalFlat * increased;
	if (verbose) { Util::SetConsoleColor(secondColor); std::cout << "\tIncreased flat damage: " << increasedFlat << std::endl; }

	intermediate = {
				total.damage.base.critChance,
				totalAttributes.agility * 0.002,
				total.per.critChancePerDex * (std::floor(totalAttributes.dexterity / 10) * 10),
				total.damage.weapon.criticalChance[total.damage.weapon.type],
				total.stacks.getCurrent(Stacks::STACK_TYPE_PHANTOM_STRIKE) * 10.0,
	};
	double unCappedCritChance = std::accumulate(intermediate.begin(), intermediate.end(), 0.0);
	double cappedCritChance = std::min<double>(unCappedCritChance, 1);
	if (verbose) {
		Util::SetConsoleColor(thirdColor);
		std::cout << "\t      + Increased crit chance: " << intermediate[0] * 100 << "%" << std::endl;
		if (intermediate[1]) std::cout << "\t      + Increased crit chance from agility: " << intermediate[1] * 100 << "%" << std::endl;
		if (intermediate[2]) std::cout << "\t      + Increased crit chance from dexterity: " << intermediate[2] * 100 << "%" << std::endl;
		if (intermediate[3]) std::cout << "\t      + Increased crit chance from weapon stance: " << intermediate[3] * 100 << "%" << std::endl;
		if (intermediate[4]) std::cout << "\t      + Increased crit chance from Phantom Strike tree node: " << intermediate[4] * 100 << "%" << std::endl;

		Util::SetConsoleColor(secondColor);
		std::cout << "\tTotal uncapped crit chance: " << unCappedCritChance * 100 << "%" << std::endl;
		std::cout << "\tTotal capped crit chance: " << cappedCritChance * 100 << "%" << std::endl;
	}

	intermediate = {
				total.damage.base.critDamage,
				totalAttributes.luck * 0.01,
				total.stacks.getCurrent(Stacks::STACK_TYPE_HIGHTENED_SENCES) * 0.25,
				total.stacks.getCurrent(Stacks::STACK_TYPE_BEAST_MODE) * 0.15,
				total.stacks.getCurrent(Stacks::STACK_TYPE_THE_ARCANE_FURY) * 0.01,
	};
	double critDamage = std::accumulate(intermediate.begin(), intermediate.end(), 0.0);
	if (verbose) {
		Util::SetConsoleColor(thirdColor);
		std::cout << "\t      + Crit damage: " << intermediate[0] * 100 << "%" << std::endl;
		if (intermediate[1]) std::cout << "\t      + Crit damage from luck: " << intermediate[1] * 100 << "%" << std::endl;
		if (intermediate[2]) std::cout << "\t      + Crit damage from Hightened Sences tree node: " << intermediate[2] * 100 << "%" << std::endl;
		if (intermediate[3]) std::cout << "\t      + Crit damage from Beast Mode tree node: " << intermediate[3] * 100 << "%" << std::endl;
		if (intermediate[4]) std::cout << "\t      + Crit damage from The Arcane Futy tree node: " << intermediate[4] * 100 << "%" << std::endl;

		Util::SetConsoleColor(secondColor);
		std::cout << "\tTotal crit damage: " << critDamage * 100 << "%" << std::endl;
	}

	double critted = increasedFlat * critDamage;
	double avgCritted = critted * cappedCritChance + increasedFlat * (1 - cappedCritChance);
	if (verbose) {
		Util::SetConsoleColor(secondColor);
		std::cout << "\tFlat damage after crit: " << critted << std::endl;
		std::cout << "\tAverage flat damage after crit: " << avgCritted << std::endl;
	}

	double elementalMulti = total.damage.elemental.getMultiplier(total.stacks);
	// too complicated to print and calculate, only print here
	if (verbose) {
		std::list<std::pair<DamageElemental::ELEMENT_TYPE, std::string>> elementalTypes = {
			{total.damage.elemental.mainType, "main"},
			{ total.damage.elemental.secondaryType, "secondary" }
		};
		for (auto type : elementalTypes)
		{
			Util::SetConsoleColor(thirdColor);
			std::cout << "\t\tCalcualating " << type.second << " damage: " << std::endl;
			switch (type.first) {
			case DamageElemental::ELEMENT_TYPE_UNDEFINED:
				break;
			case DamageElemental::ELEMENT_TYPE_FIRE:
				if (total.damage.elemental.fire) std::cout << "\t\t      + Fire damage: " << total.damage.elemental.fire * 100 - 100 << "%" << std::endl;
				if (total.stacks.getCurrent(Stacks::STACK_TYPE_FIRE_STARTER)) std::cout << "\t\t      + Fire damage from Fire Starter tree node: "
					<< total.stacks.getCurrent(Stacks::STACK_TYPE_FIRE_STARTER) * 0.01 * 100 << "%" << std::endl;
				break;
			case DamageElemental::ELEMENT_TYPE_LIGHTNING:
				if (total.damage.elemental.lightning) std::cout << "\t\t      + Lightning damage: " << total.damage.elemental.lightning * 100 - 100 << "%" << std::endl;
				if (total.stacks.getCurrent(Stacks::STACK_TYPE_FIRE_STARTER)) std::cout << "\t\t      + Fire damage from Fire Starter tree node: "
					<< total.stacks.getCurrent(Stacks::STACK_TYPE_TASTE_MY_LIGHTNING) * 0.01 * 100 << "%" << std::endl;
				break;
			case DamageElemental::ELEMENT_TYPE_ARCANE:
				if (total.damage.elemental.arcane) std::cout << "\t\t      + Fire damage: " << total.damage.elemental.arcane * 100 - 100 << "%" << std::endl;
				if (total.stacks.getCurrent(Stacks::STACK_TYPE_FIRE_STARTER)) std::cout << "\t\t      + Fire damage from Fire Starter tree node: "
					<< total.stacks.getCurrent(Stacks::STACK_TYPE_THE_ARCANE_FURY) * 0.01 * 100 << "%" << std::endl;
				break;
			}

			if (total.stacks.getCurrent(Stacks::STACK_TYPE_DEFIANCE)) std::cout << "\t\t      + Fire damage from Defiance tree node: "
				<< total.stacks.getCurrent(Stacks::STACK_TYPE_DEFIANCE) * 0.1 * 100 << "%" << std::endl;
			if (total.stacks.getCurrent(Stacks::STACK_TYPE_THE_ELEMENTALIST)) std::cout << "\t\t      + Fire damage from The Elelemtalist tree node: "
				<< total.stacks.getCurrent(Stacks::STACK_TYPE_THE_ELEMENTALIST) * 0.5 * 100 << "%" << std::endl;
		}

		Util::SetConsoleColor(secondColor); std::cout << "\tTotal more Elemental damage: " << elementalMulti * 100 - 100 << "%" << std::endl;
	}

	intermediate = {
			elementalMulti,
			total.damage.offhand.damage[total.damage.offhand.type],
			total.damage.offhand.bonusDamage[total.damage.offhand.type],
			total.stacks.getCurrent(Stacks::STACK_TYPE_MONSTER_HUNTER) * 0.1,
			total.stacks.getCurrent(Stacks::STACK_TYPE_BEAST_MODE) * 0.1,
	};
	double multiply = intermediate[0] * intermediate[1] * (intermediate[2] + intermediate[3] + intermediate[4]);
	double multiplied = avgCritted * multiply;
	if (verbose) {
		Util::SetConsoleColor(thirdColor);
		std::cout << "\t      * More Elemental damage: " << intermediate[0] * 100 - 100 << "%" << std::endl;
		std::cout << "\t      * More Offhand damage: " << intermediate[1] * 100 - 100 << "%" << std::endl;
		std::cout << "\t      * Increased Boss damage: " << intermediate[2] * 100 - 100 << "%" << std::endl;
		if (intermediate[3]) std::cout << "\t              + Increased Boss damage from Monster Hunter tree node: " << intermediate[3] * 100 << "%" << std::endl;
		if (intermediate[4]) std::cout << "\t              + Increased Boss damage from Beast Mode tree node: " << intermediate[4] * 100 << "%" << std::endl;

		Util::SetConsoleColor(secondColor);
		std::cout << "\tTotal more damage: " << multiply * 100 - 100 << "%" << std::endl;
		std::cout << "Total increased and multiplied damage: " << multiplied << std::endl;

		Util::ResetConsoleColor();
	}

	return multiplied;
}

double Calculator::getSurvivalRating() {
	const auto secondColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	const auto thirdColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	if (verbose) { std::cout << std::endl << "Explaining survivability:" << std::endl; }

	std::vector<double> intermediate = {
			total.survivability.maxHealth,
			totalAttributes.stamina * 5,
			total.survivability.healthBonus,
			total.stacks.getCurrent(Stacks::STACK_TYPE_GRAND_VITALITY) * 0.1,
			total.stacks.getCurrent(Stacks::STACK_TYPE_CLARITY) * 0.1,
	};
	double health = (intermediate[0] + intermediate[1]) * (intermediate[2] + intermediate[3] + intermediate[4]);
	if (verbose) {
		Util::SetConsoleColor(thirdColor);
		std::cout << "\t      + Max health: " << intermediate[0] << std::endl;
		std::cout << "\t      + Max healt from stamina: " << intermediate[1] << std::endl;
		std::cout << "\t      * Health Bonus: " << intermediate[2] * 100 - 100 << "%" << std::endl;
		if (intermediate[3]) std::cout << "\t              + Health Bonus from Vitality tree node: " << intermediate[3] * 100 - 100 << "%" << std::endl;
		if (intermediate[4]) std::cout << "\t              + Health Bonus from Clarity Mode tree node: " << intermediate[4] * 100 - 100 << "%" << std::endl;

		Util::SetConsoleColor(secondColor);
		std::cout << "\tTotal health: " << health << std::endl;
	}

	intermediate = {
			total.survivability.healthRegen,
			totalAttributes.stamina * 0.03,
			total.survivability.hpRegenBonus,
			total.stacks.getCurrent(Stacks::STACK_TYPE_ENVIGORATING_GUST) * 0.05,
			total.stacks.getCurrent(Stacks::STACK_TYPE_TOLERANCE) * 0.1,
			total.stacks.getCurrent(Stacks::STACK_TYPE_CLARITY) * 0.1,
	};
	double regen = (intermediate[0] + intermediate[1]) * (intermediate[2] + intermediate[3] + intermediate[4] + intermediate[5]);
	double preudoHp = (health + REGEN_WEIGHT * regen);
	if (verbose) {
		Util::SetConsoleColor(thirdColor);
		
		std::cout << "\t      + Health Regen: " << intermediate[0] << std::endl;
		std::cout << "\t      + Health Regen from stamina: " << intermediate[1] << std::endl;
		std::cout << "\t      * Health Regen Bonus: " << intermediate[2] * 100 - 100 << "%" << std::endl;
		if (intermediate[3]) std::cout << "\t/t      + Health Regen Bonus from Envigorating Gust tree node: " << intermediate[3] * 100 - 100 << "%" << std::endl;
		if (intermediate[4]) std::cout << "\t/t      + Health Regen Bonus from Tolerance tree node: " << intermediate[4] * 100 - 100 << "%" << std::endl;
		if (intermediate[5]) std::cout << "\t/t      + Health Regen Bonus from Clarity tree node: " << intermediate[5] * 100 - 100 << "%" << std::endl;
		

		Util::SetConsoleColor(secondColor);
		std::cout << "\tTotal Health Regen: " << regen << std::endl;
		Util::SetConsoleColor(thirdColor);
		std::cout << "\t      * Health Regen EHP multiplier: (total regen * " << REGEN_WEIGHT << ") is added as EHP" << std::endl;
		Util::SetConsoleColor(secondColor);
		std::cout << "\tTotal Max Health and Health Regen: " << preudoHp << std::endl;
	}

	intermediate = {
			total.survivability.armor,
			totalAttributes.dexterity,
			total.survivability.armorBonus,
			total.stacks.getCurrent(Stacks::STACK_TYPE_DEFIANCE) * 0.5,
			total.stacks.getCurrent(Stacks::STACK_TYPE_GRAND_VITALITY) * 0.1,
			total.stacks.getCurrent(Stacks::STACK_TYPE_MONSTER_HUNTER) * 0.05,
			total.stacks.getCurrent(Stacks::STACK_TYPE_TASTE_MY_LIGHTNING) * 0.01,
			total.stacks.getCurrent(Stacks::STACK_TYPE_ANCESTRAL_BARRIER) * 0.5,
	};
	double armour = (intermediate[0] + intermediate[1]) * (intermediate[2] + intermediate[3] + intermediate[4] + intermediate[5] + intermediate[6] + intermediate[7]);
	if (verbose) {
		Util::SetConsoleColor(thirdColor);
		std::cout << "\t      + Armor: " << intermediate[0] << std::endl;
		std::cout << "\t      + Armor from dexterity: " << intermediate[1] << std::endl;
		std::cout << "\t      * Armor Bonus: " << intermediate[2] * 100 - 100 << "%" << std::endl;
		if (intermediate[3]) std::cout << "\t              + Health Bonus from Defiance tree node: " << intermediate[3] * 100 - 100 << "%" << std::endl;
		if (intermediate[4]) std::cout << "\t              + Health Bonus from Grand Vitality tree node: " << intermediate[4] * 100 - 100 << "%" << std::endl;
		if (intermediate[5]) std::cout << "\t              + Health Bonus from Monster Hunter tree node: " << intermediate[5] * 100 - 100 << "%" << std::endl;
		if (intermediate[6]) std::cout << "\t              + Health Bonus from Tase My Lightning tree node: " << intermediate[6] * 100 - 100 << "%" << std::endl;
		if (intermediate[7]) std::cout << "\t              + Health Bonus from Ancestral Barrier tree node: " << intermediate[7] * 100 - 100 << "%" << std::endl;

		Util::SetConsoleColor(secondColor);
		std::cout << "\tTotal Armor: " << armour << std::endl;
	}

	const double maxArmourDr = 80;
	const double ruptureArmourConstant = 1922.122; // https://mycurvefit.com/ 5PL Assymetrical sigmoidal
	double armourDr = maxArmourDr - maxArmourDr / (1 + armour / ruptureArmourConstant);
	double dr = std::min<double>(total.survivability.damageReduction + total.stacks.getCurrent(Stacks::STACK_TYPE_ENVIGORATING_GUST) * 0.05, 0.5); // max 50% dr
	double ehp = preudoHp
		/ (1 - dr)
		/ (1 - armourDr / 100);

	if (verbose) {
		Util::SetConsoleColor(secondColor);
		std::cout << "\t\tCurrent rupture: " << 110 << std::endl;
		std::cout << "\tCurrent Armor DR: " << armourDr << "%" << std::endl;
		std::cout << "\tCurrent DR: " << dr * 100 << "%" << std::endl;

		Util::ResetConsoleColor();
		std::cout << "Total ehp: " << ehp << std::endl;
	}

	return ehp;
}

Calculator Calculator::copy() {
	Calculator calcualtor;
	calcualtor.init = init;
	calcualtor.stats = stats;
	calcualtor.cards = cards;
	calcualtor.types = types;
	calcualtor.prediction = prediction;
	calcualtor.total = total;
	calcualtor.totalAttributes = totalAttributes;
	return calcualtor;
}