#pragma once

#include "Stats/Stats.h"

#include <string>
#include <vector>

class ItemParser {
	static std::vector<std::string> knownStats;

	static std::string ToLower(const std::string& str);
	static int LevenshteinDistance(const std::string& s1, const std::string& s2);
	static std::string FindClosestStatName(const std::string& input);
	static std::string Trim(const std::string& str);
	static bool ExtractNumber(std::string& line, double& value);
	static std::wstring StringToWString(const std::string& str);
	static std::string WStringToString(const std::wstring& wstr);

public:
	static int verbose;
	static void ApplyStat(Stats& stats, const std::string& statName, double value);
	static std::vector<Stats> ParseStatsFromFile(const std::string& filename);
	static Stats ParseCardsFromFile(const std::string& filename);
	static std::vector<std::string> FindPyFilesInFolder(const std::string& folderPath);
};