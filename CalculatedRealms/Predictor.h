#pragma once

#include "Calculator.h"

#include <vector>
#include <list>
#include <string>
#include <iostream>

class Predictor {
public:
	typedef std::pair<std::string, double> PredictedStat;
	typedef std::list<PredictedStat> PredictedStats;
private:
	static const std::vector<PredictedStats> treeNodeList;
	static const std::vector<PredictedStats> attributesList;
	static const std::vector<PredictedStats> cardList;

	static std::map<std::string, int> parsedCards;

public:
	static std::string statListStr(const PredictedStats& statList, bool withValue = 1);

	static void predict(Calculator& calculator, const std::vector<Predictor::PredictedStats>& predictList);

	static void predictTree(Calculator& calculator) { std::cout << "Predict tree: " << std::endl; predict(calculator, treeNodeList); }

	static void predictAttributes(Calculator& calculator) { std::cout << "Predict attributes: " << std::endl; predict(calculator, attributesList); }

	static void predictCards(Calculator& calculator) { std::cout << "Predict cards: " << std::endl; predict(calculator, cardList); }
	static void printCards();
	static const std::vector<PredictedStats>& getCardList() { return cardList; }

	static void resetParsedCards() { for (const auto& card : cardList) parsedCards[statListStr(card)] = 0; }
	static void parseCard(const Predictor::PredictedStats& card, int count) { parsedCards[statListStr(card)] = count; }
	static int parsedCard(const Predictor::PredictedStats& card) { return parsedCards[statListStr(card)]; }
};