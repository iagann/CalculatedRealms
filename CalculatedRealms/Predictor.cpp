#include "Predictor.h"
#include "ItemParser.h"
#include "Util.h"
#include "CardReroller.h"

#include <sstream>
#include <iomanip>  // Include this header for setprecision and fixed
#include <algorithm>  // For std::sort

std::map<std::string, int> Predictor::parsedCards = std::map<std::string, int>();

const std::vector<Predictor::PredictedStats> Predictor::treeNodeList = {
	{ { "Health Bonus", 1 } },
	{ { "Health Bonus", 3 } },
	{ { "Damage Bonus", 1 } },
	{ { "Damage Bonus", 2 } },
	{ { "Agility Bonus", 2 } },
	{ { "Strength Bonus", 2 } },
	{ { "Endurance Bonus", 2 } },
	{ { "Armor Bonus", 5 } },
	{ { "HP Regen Bonus", 0.25 } },
	{ { "Crit Chance Bonus", 1 } },
	{ { "Energy Regen Bonus", 2 } },
	{ { "Crit Damage Bonus", 0.5 } },
	{ { "Boss Damage Bonus", 3 } },
	{ { "Inventory Slots", 6 } },
	{ { "Lifesteal Chance", 1 } },
	{ { "Lifesteal Amount", 1 } },
	{ { "Experience Bonus", 3 } },

	{ { "Fire Damage Bonus", 3 } },
	{ { "Lightning Damage Bonus", 3 } },
	{ { "Arcane Damage Bonus", 3 } },
	{ { "Fire Damage Bonus", 10 } },
	{ { "Lightning Damage Bonus", 10 } },
	{ { "Arcane Damage Bonus", 10 } },
	{ { "Fire Damage Bonus", 25 } },
	{ { "Lightning Damage Bonus", 25 } },
	{ { "Arcane Damage Bonus", 25 } },
	
	// hybrid
	{ { "Crit Chance Bonus", 1 }, { "Crit Damage Bonus", 0.5 } },
	{ { "Strength Bonus", 1 }, { "Damage Bonus", 0.5 } },
	{ { "HP Regen Bonus", 0.15 }, { "Energy Regen Bonus", 1 } },
	{ { "Armor Bonus", 2 }, { "Health Bonus", 2 } },
	
	{ { "Warstrike", 1} },
	{ { "Defiance", 1} },
	{ { "Colossal Force", 1} },
	{ { "Envigorating Gust", 1} },
	{ { "Phantom Strike", 1 } },
	{ { "Tolerance", 1 } },
	{ { "Grand Vitality", 1 } },
	{ { "Slice 'n Dice", 1 } },
	{ { "Infinite Ward", 1 } },
	{ { "Hightened Sences", 1 } },
	{ { "Monster Hunter", 1 } },
	{ { "Carrier has Arrived", 1 } },
	{ { "Clarity", 1 } },
	{ { "Beast Mode", 1 } },
	{ { "Soul of the Prophet", 1 } },
	{ { "The Elementalist", 1 } },
	{ { "Strength and Honor", 1 } },
	{ { "Arcane Fury", 100 } },
	{ { "Taste my Lightning", 100 } },
	{ { "Fire Starter", 100 } },
	{ { "Ancestral Barrier", 1 } },
	
};

const std::vector<Predictor::PredictedStats> Predictor::attributesList = {
	{ { "Strength", 100 }, { "Character level", 100 } },
	{ { "Agility", 100 }, { "Character level", 100 } },
	{ { "Stamina", 100 }, { "Character level", 100 } },
	{ { "Wisdom", 100 }, { "Character level", 100 } },
	{ { "Endurance", 100 }, { "Character level", 100 } },
	{ { "Luck", 100 }, { "Character level", 100 } },
	{ { "Dexterity", 100 }, { "Character level", 100 } },
};

const std::vector<Predictor::PredictedStats> Predictor::cardList = {
	{ { "Inventory Slots", 4 }, { "Arcane Damage Bonus", 10 } },
	{ { "Inventory Slots", 4 }, { "Damage Bonus", 10 } },
	{ { "Inventory Slots", 4 }, { "Fire Damage Bonus", 10 } },
	{ { "Inventory Slots", 4 }, { "Lightning Damage Bonus", 10 } },
	{ { "Armor", 100 }, { "Arcane Damage Bonus", 15 } },
	{ { "Armor", 100 }, { "Damage Bonus", 15 } },
	{ { "Armor", 100 }, { "Fire Damage Bonus", 15 } },
	{ { "Armor", 100 }, { "Lightning Damage Bonus", 15 } },
	{ { "Armor", 75 }, { "Armor Bonus", 10 } },
	{ { "Boss Damage", 10 }, { "Damage", 10 } },
	{ { "Crit Chance", 2 }, { "Agility", 10 } },
	{ { "Crit Damage", 15 }, { "Agility", 10 } },
	{ { "Crit Damage", 15 }, { "Crit Chance", 3 } },
	{ { "Crit Damage", 15 }, { "Dexterity", 10 } },
	{ { "Crit Damage", 15 }, { "Endurance", 10 } },
	{ { "Crit Damage", 15 }, { "Luck", 10 } },
	{ { "Crit Damage", 15 }, { "Strength", 10 } },
	{ { "Crit Damage", 15 }, { "Wisdom", 10 } },
	{ { "Crit Damage", 25 }, { "Strength", 5 } },
	{ { "Damage", 10 }, { "Arcane Damage Bonus", 15 } },
	{ { "Damage", 10 }, { "Fire Damage Bonus", 15 } },
	{ { "Damage", 10 }, { "Lightning Damage Bonus", 15 } },
	{ { "Damage", 10 }, { "Strength Bonus", 15 } },
	{ { "Damage", 20 } },
	{ { "Damage Bonus", 5 }, { "Strength", 10 } },
	{ { "Damage Bonus", 15 }, { "Max Health", 75 } },
	{ { "Damage Bonus", 25 }, { "Agility", 10 } },
	{ { "Damage Bonus", 25 }, { "Dexterity", 10 } },
	{ { "Damage Bonus", 25 }, { "Endurance", 10 } },
	{ { "Damage Bonus", 25 }, { "Luck", 10 } },
	{ { "Damage Bonus", 25 }, { "Stamina", 10 } },
	{ { "Damage Bonus", 25 }, { "Strength", 10 } },
	{ { "Damage Bonus", 25 }, { "Wisdom", 10 } },
	{ { "Damage Reduction", 1 }, { "Armor", 50 } },
	{ { "Damage Reduction", 1 }, { "Damage", 10 } },
	{ { "Dexterity", 10 }, { "Attack Speed", 60 } },
	{ { "Dexterity", 15 }, { "Attack Speed", 75 } },
	{ { "Energy Regen", 3 }, { "Damage", 10 } },
	{ { "Energy Regen", 3 }, { "Max Energy", 3 } },
	{ { "XP Bonus", 25 }, { "Agility Bonus", 15 } },
	{ { "XP Bonus", 25 }, { "Dexterity Bonus", 15 } },
	{ { "XP Bonus", 25 }, { "Endurance Bonus", 15 } },
	{ { "XP Bonus", 25 }, { "Luck Bonus", 15 } },
	{ { "XP Bonus", 25 }, { "Stamina Bonus", 15 } },
	{ { "XP Bonus", 25 }, { "Strength Bonus", 15 } },
	{ { "Health Regen", 1 }, { "Damage", 10 } },
	{ { "Max Health Bonus", 10 } },
	{ { "Max Health", 60 }, { "Stamina Bonus", 10 } },
	{ { "Max Health", 60 }, { "Wisdom", 10 } },
	{ { "Strength", 10 }, { "Agility", 10 }, { "Stamina", 10 } },
};

std::string Predictor::statListStr(const PredictedStats& statList, bool withValue) {
	std::stringstream ss;
	bool first = true;
	if (statList.size() == 1) {
		bool found = false;
		for (const auto& stack : Stacks::getStacksMap()) {
			if (statList.begin()->first.find(stack.second) != std::string::npos)
				return "notable " + stack.second;
		}
	}

	for (const auto& stat : statList) {
		if (!first)
			ss << ", ";
		ss << stat.first;
		if (withValue)
			ss << " = " << stat.second;
		first = false;
	}
	return ss.str();
}

void Predictor::predict(Calculator& calculator, const std::vector<Predictor::PredictedStats>& predictList) {
	double currentRating = calculator.getRatingSimple();

	std::vector<std::pair<double, std::string>> predictions;

	/*
	for (int i = 0; i < treeNodeList.size(); ++i) {
		Stats stats;
		const PredictedStats& predictedStats = treeNodeList[i];
		for (const auto& stat : predictedStats) {
			ItemParser::ApplyStat(stats, stat.first, stat.second);
		}
		calculator.setPrediction(stats);
		double predictedRating = calculator.getRating();
		std::stringstream ss;
		double predictedIncrease = (predictedRating - currentRating) / currentRating * 100;
		if (predictedIncrease == 0)
			continue;
		ss << std::setw(10) << std::setprecision(2) << predictedIncrease << "%" << " = " << statListStr(predictedStats);
		predictions.push_back(std::make_pair(predictedIncrease, ss.str()));
	}
	*/

	bool predictingCards = predictList == cardList;
	
#pragma omp parallel for num_threads(Util::getMaxThreads())
	for (int i = 0; i < predictList.size(); ++i) {
		Stats stats;
		const PredictedStats& predictedStats = predictList[i];

		// Create a copy of the calculator object for each iteration
		Calculator calculatorCopy = calculator.copy();

		// Thread-safe stringstream and pair to avoid race conditions
		std::stringstream ss;
		std::vector<std::pair<double, std::string>> local_predictions;  // Local container for thread safety

		int k = 1;
		if (predictingCards && parsedCard(predictedStats) > 3) {
			continue; // more than 3 cards mean a full set, no need to predict
		}
		else if (predictingCards && parsedCard(predictedStats) == 3) {
			k = 3; // if card count is already 3, then next card pick is equal to 6 total cards, 6 - 3 = 3
		}
		for (const auto& predictedStat : predictedStats) {
			ItemParser::ApplyStat(stats, predictedStat.first, predictedStat.second * k);
		}

		calculatorCopy.setPrediction(stats);
		double predictedRating = calculatorCopy.getRatingSimple();
		double predictedIncrease = (predictedRating - currentRating) / currentRating * 100;

		/*
		if (predictedIncrease == 0)
			continue;
		*/

		ss << std::setw(10) << std::setprecision(3) << predictedIncrease << "%" << " = " << statListStr(predictedStats);
		//	<< " = " << std::fixed << std::setprecision(17) << predictedRating;
		local_predictions.push_back(std::make_pair(predictedIncrease, ss.str()));

		// Combine results from each thread into the shared predictions vector
#pragma omp critical
		predictions.insert(predictions.end(), local_predictions.begin(), local_predictions.end());
	}

	// Sort the vector by the first element of the pair in descending order
	std::sort(predictions.begin(), predictions.end(),
		[](const std::pair<double, std::string>& a, const std::pair<double, std::string>& b) {
			return a.first > b.first;  // Descending order
		});

	if (predictingCards) {
		//std::cout << "Card reroll strategy: " << std::endl;
		Util::SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		CardReroller::printStrategy(predictions, 10);
		Util::ResetConsoleColor();
		//std::cout << std::endl;
	}

	int i = 1;
	for (const auto& prediction : predictions) {
		if (prediction.second.find("notable ") != std::string::npos) {
			Util::SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else {
			Util::SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}
		std::cout << i++ << prediction.second << std::endl;
	}

	return;
}

void Predictor::printCards() {
	std::cout << "CARDS START" << std::endl;
	std::cout << "type: cards" << std::endl;
	for (const PredictedStats& stats : cardList) {
		std::cout << '"' << statListStr(stats) << '"' << " 0" << std::endl;
	}
	std::cout << "CARDS END" << std::endl;
}