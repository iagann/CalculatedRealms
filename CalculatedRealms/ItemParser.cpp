#include "ItemParser.h"
#include "Stats/Stacks.h"
#include "Util.h"
#include "Predictor.h"

#include <windows.h>
#include <fstream>
#include <map>
#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <omp.h>

#define MIN_DISTANCE 3

int ItemParser::verbose = 0;

// Utility function to convert a string to lowercase
std::string ItemParser::ToLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}

// Utility function to calculate Levenshtein distance
int ItemParser::LevenshteinDistance(const std::string& s1, const std::string& s2) {
    std::string lowerS1 = ToLower(s1);
    std::string lowerS2 = ToLower(s2);

    const size_t len1 = lowerS1.size(), len2 = lowerS2.size();
    std::vector<std::vector<int>> d(len1 + 1, std::vector<int>(len2 + 1));

    d[0][0] = 0;
    for (size_t i = 1; i <= len1; ++i) d[i][0] = i;
    for (size_t i = 1; i <= len2; ++i) d[0][i] = i;

    for (size_t i = 1; i <= len1; ++i)
        for (size_t j = 1; j <= len2; ++j)
            d[i][j] = std::min<int>({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + (lowerS1[i - 1] == lowerS2[j - 1] ? 0 : 1) });

    return d[len1][len2];
}

// Function to find the closest matching stat name
std::string ItemParser::FindClosestStatName(const std::string& statName) {
    int minDistance = INT_MAX;
    static std::vector<std::string> knownStats = {
        "Item level", "Armor", "Armor Bonus", "Damage", "Fire Damage Bonus", "Arcane Damage Bonus", "Lightning Damage Bonus",
        "Critical Chance", "Crit Chance", "Crit Chance Bonus", "Critical Damage Bonus", "Crit Damage", "Crit Damage Bonus", "Attack Speed",
        "Increased Damage", "Lifesteal Chance", "Lifesteal Amount", "Life Steal",
        "Boss Damage", "Boss Damage Bonus", "XP Bonus", "Inventory Slots",
        "Gain  CRiTicaL CHANCE FOR EACH 10 DEXTERITY", "FOR EACH  POINTS in DEXTERITY GAIN 1% DAMAGE", "FOR EACH  POINTS in ENDURANCE GAinN 10",
        "FIRE ABILITIES WILL ALSO BENEFIT FROM LIGHTNING",
        "FIRE ABILITIES WILL ALSO BENEFIT FROM ARCANE",
        "THIS AURA GRANTS YOU FIRE DAMAGE BUFF",
        "BONUS FIRE DAMAGE FOR A SHORT DURATION.",
        "EXTRA INVENTORY SLOT",
        "Energy Regeneration", "Gain  ENERGY REGENERATION", "Life Steal Chance", "Max Health", "Max Health Bonus", "Max Energy", "Health Bonus",
        "HP Regen Bonus", "Health Regen", "Energy Regen Bonus", "Energy Regen", "Health Regeneration", "Character level",
        "Strength", "Agility", "Stamina", "Endurance", "Luck", "Dexterity", "Wisdom",
        "Strength Bonus", "Agility Bonus", "Stamina Bonus", "Endurance Bonus", "Luck Bonus", "Dexterity Bonus", "Wisdom Bonus",
        "Damage Bonus", "Experience Bonus", "Damage Reduction", "Damage Reduction Bonus",
        "GET  ATTACK SPEED PER MAX POTION SLOTS", "Gain  CRIT DAMAGE FOR EVERY 100 ARMOR", "FOR EACH AVAILABLE POTION GET  DAMAGE",
        "FOR EACH AVAILABLE POTION GET  ARMOUR", "GaAin ADDITIONAL  DAMAGE BONUS FOR EACH", "Extra Potion Slot",
        "Gain  CRiTicaL CHANCE FOR EVERY 10",

    };

    // Use a static boolean to prevent repeated modifications
    static bool knownStatsIsInitialized = false;
    if (!knownStatsIsInitialized) {
        for (const auto& offhand : DamageOffhand::getOffhandMap()) {
            knownStats.push_back(offhand.second + " Damage");
            knownStats.push_back(offhand.second + " Damage Bonus");
        }
        for (const auto& weapon : DamageWeapon::getWeaponMap()) {
            knownStats.push_back(weapon.second + " Damage");
            knownStats.push_back(weapon.second + " Critical Chance");
            knownStats.push_back(weapon.second + " Critical Damage");
        }
        for (const auto& stack : Stacks::getStacksMap()) {
            knownStats.push_back(stack.second);
        }
        knownStatsIsInitialized = true; // Ensure this block runs only once
    }

    std::string bestMatch;

    for (const auto& stat : knownStats) {
        int distance = LevenshteinDistance(statName, stat);
        if (distance < minDistance) {
            minDistance = distance;
            bestMatch = stat;
        }
    }

    // Set a threshold for acceptable similarity
    if (minDistance <= std::max<double>(MIN_DISTANCE, statName.length() / 10)) { // Adjust threshold as needed
        return bestMatch;
    }

    Util::ResetConsoleColor();
    std::cerr << "\t\tStat not found: '" << statName << "'" << std::endl;
    return ""; // Return empty if no suitable match
}

// Utility function to trim leading and trailing whitespace
std::string ItemParser::Trim(const std::string& str) {
    std::string s = str;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] < -1 || s[i] > 255)
            s[i] = ' ';
    }
    size_t first = s.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = s.find_last_not_of(' ');
    return s.substr(first, last - first + 1);
}

// Function to extract a number from a string and remove it from the string
bool ItemParser::ExtractNumber(std::string& line, double& value) {
    std::string number;
    bool found = false;

    // Extract characters that are part of a valid number
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c >= -1 && c <= 255 && isdigit(c) || (c == '.' && found)
            || (i + 1 < line.size() && (c == '-' || c == '+')) && isdigit(line[i+1])) {
            number += c;
            found = true;
        }
        else if (found && !isdigit(c) && c != '.') {
            // Stop extracting when a non-numeric character is found after a number
            break;
        }
    }

    if (found && !number.empty()) {
        try {
            value = std::stod(number);
            // Remove the extracted number from the line
            line.erase(line.find(number), number.length());
            return true;
        }
        catch (const std::invalid_argument&) {
            return false;
        }
    }

    return false;
}

void ItemParser::ApplyStat(Stats& stats, const std::string& statName, double value = 1) {
    if (statName == "Item level")
        return;

    static std::map<std::string, std::function<void(Stats& stats, double)>> statActions = {
        // attributes
        {"Strength", [&](Stats& stats, double value) { stats.attributes.base.strength += value; }},
        {"Agility", [&](Stats& stats, double value) { stats.attributes.base.agility += value; }},
        {"Stamina", [&](Stats& stats, double value) { stats.attributes.base.stamina += value; }},
        {"Endurance", [&](Stats& stats, double value) { stats.attributes.base.endurance += value; }},
        {"Luck", [&](Stats& stats, double value) { stats.attributes.base.luck += value; }},
        {"Dexterity", [&](Stats& stats, double value) { stats.attributes.base.dexterity += value; }},
        {"Wisdom", [&](Stats& stats, double value) { stats.attributes.base.wisdom += value; }},
        {"Strength Bonus", [&](Stats& stats, double value) { stats.attributes.bonus.strength += value / 100; }},
        {"Agility Bonus", [&](Stats& stats, double value) { stats.attributes.bonus.agility += value / 100; }},
        {"Stamina Bonus", [&](Stats& stats, double value) { stats.attributes.bonus.stamina += value / 100; }},
        {"Endurance Bonus", [&](Stats& stats, double value) { stats.attributes.bonus.endurance += value / 100; }},
        {"Luck Bonus", [&](Stats& stats, double value) { stats.attributes.bonus.luck += value / 100; }},
        {"Dexterity Bonus", [&](Stats& stats, double value) { stats.attributes.bonus.dexterity += value / 100; }},
        {"Wisdom Bonus", [&](Stats& stats, double value) { stats.attributes.bonus.wisdom += value / 100; }},

        // damage
        {"Damage", [&](Stats& stats, double value) { stats.damage.base.flat += value; }},
        {"Fire Damage Bonus", [&](Stats& stats, double value) { stats.damage.elemental.fire += value / 100; }},
        {"Lightning Damage Bonus", [&](Stats& stats, double value) { stats.damage.elemental.lightning += value / 100; }},
        {"Arcane Damage Bonus", [&](Stats& stats, double value) { stats.damage.elemental.arcane += value / 100; }},
        {"Critical Chance", [&](Stats& stats, double value) { stats.damage.base.critChance += value / 100; }},
        {"Crit Chance", [&](Stats& stats, double value) { stats.damage.base.critChance += value / 100; }},
        {"Crit Chance Bonus", [&](Stats& stats, double value) { stats.damage.base.critChance += value / 100; }},
        {"Critical Damage Bonus", [&](Stats& stats, double value) { stats.damage.base.critDamage += value / 100; }},
        {"Crit Damage", [&](Stats& stats, double value) { stats.damage.base.critDamage += value / 100; }},
        {"Crit Damage Bonus", [&](Stats& stats, double value) { stats.damage.base.critDamage += value / 100; }},
        {"Attack Speed", [&](Stats& stats, double value) { stats.damage.base.attackSpeed += value; }},
        {"Increased Damage", [&](Stats& stats, double value) { stats.damage.base.increased += value / 100; }},
        {"Damage Bonus", [&](Stats& stats, double value) { stats.damage.base.increased += value / 100; }},
        {"Lifesteal Chance", [&](Stats& stats, double value) { stats.survivability.lifeStealChance += value / 100; }},
        {"Lifesteal Amount", [&](Stats& stats, double value) { stats.survivability.lifeSteal += value / 100; }},
        {"Life Steal", [&](Stats& stats, double value) { stats.survivability.lifeSteal += value / 100; }},
        

        // survivability (mostly)
        {"Armor", [&](Stats& stats, double value) { stats.survivability.armor += value; }},
        {"Armor Bonus", [&](Stats& stats, double value) { stats.survivability.armorBonus += value / 100; }},
        {"Energy Regeneration", [&](Stats& stats, double value) { stats.survivability.energyRegen += value; }},
        {"Gain  ENERGY REGENERATION", [&](Stats& stats, double value) { stats.survivability.energyRegen += value; }},
        {"Life Steal Chance", [&](Stats& stats, double value) { stats.survivability.lifeStealChance += value / 100; }},
        {"Max Health", [&](Stats& stats, double value) { stats.survivability.maxHealth += value; }},
        {"Character level", [&](Stats& stats, double value) { stats.survivability.maxHealth += value * 5; }},
        {"Health Bonus", [&](Stats& stats, double value) { stats.survivability.healthBonus += value / 100; }},
        {"Max Health Bonus", [&](Stats& stats, double value) { stats.survivability.healthBonus += value / 100; }},
        {"Health Regen", [&](Stats& stats, double value) { stats.survivability.healthRegen += value; }},
        {"Health Regeneration", [&](Stats& stats, double value) { stats.survivability.healthRegen += value; }},
        {"HP Regen Bonus", [&](Stats& stats, double value) { stats.survivability.hpRegenBonus += value / 100; }},
        {"Max Energy", [&](Stats& stats, double value) { stats.survivability.maxEnergy += value; }},
        {"Energy Regen", [&](Stats& stats, double value) { stats.survivability.energyRegenBonus += value / 100; }},
        {"Energy Regen Bonus", [&](Stats& stats, double value) { stats.survivability.energyRegenBonus += value / 100; }},
        {"Damage Reduction", [&](Stats& stats, double value) { stats.survivability.damageReduction += value / 100; }},
        {"Damage Reduction Bonus", [&](Stats& stats, double value) { stats.survivability.damageReduction += value / 100; }},

        // other
        {"Boss Damage", [&](Stats& stats, double value) { stats.damage.base.bossDamage += value / 100; }},
        {"Boss Damage Bonus", [&](Stats& stats, double value) { stats.damage.base.bossDamage += value / 100; }},
        {"XP Bonus", [&](Stats& stats, double value) { stats.xpBonus += value / 100; }},
        {"Experience Bonus", [&](Stats& stats, double value) { stats.xpBonus += value / 100; }},
        {"Inventory Slots", [&](Stats& stats, double value) { stats.extraInventorySlots += value; }},
        {"Extra Potion Slot", [&](Stats& stats, double value) { stats.potionSlots += value; }},

        // special
        {"FOR EACH  POINTS in DEXTERITY GAIN 1% DAMAGE", [&](Stats& stats, double value) { stats.per.increasedDamagePerDex += 0.01 / 10; }},
        {"Gain  CRiTicaL CHANCE FOR EACH 10 DEXTERITY", [&](Stats& stats, double value) { stats.per.critChancePerDex += 0.01 / 10; }},
        {"Gain  CRiTicaL CHANCE FOR EVERY 10", [&](Stats& stats, double value) { stats.per.critChancePerDex += 0.01 / 10; }},
        {"GaAin ADDITIONAL  DAMAGE BONUS FOR EACH", [&](Stats& stats, double value) { stats.per.damagePerExtraInventorySlot += 0.01; }},
        {"FOR EACH  POINTS in ENDURANCE GAinN 10", [&](Stats& stats, double value) { stats.per.damagePerEndurance += 10 / 150; }},
        {"GET  ATTACK SPEED PER MAX POTION SLOTS", [&](Stats& stats, double value) { stats.per.attackSpeedPerPotionSlot += value * 0.05; }},
        {"FOR EACH AVAILABLE POTION GET  DAMAGE", [&](Stats& stats, double value) { stats.per.damagePerPotionSlot += value / 100; }},
        {"FOR EACH AVAILABLE POTION GET  ARMOUR", [&](Stats& stats, double value) { stats.per.armourPerPotionSlot += value; }},
        {"Gain  CRIT DAMAGE FOR EVERY 100 ARMOR", [&](Stats& stats, double value) { stats.per.critDamagePerArmour += value * 0.01 / 100; }},
      
        // dragon
        {"FIRE ABILITIES WILL ALSO BENEFIT FROM LIGHTNING", [&](Stats& stats, double value) { stats.damage.elemental.mainType = DamageElemental::ELEMENT_TYPE_FIRE; stats.damage.elemental.secondaryType = DamageElemental::ELEMENT_TYPE_LIGHTNING; }},
        {"FIRE ABILITIES WILL ALSO BENEFIT FROM ARCANE", [&](Stats& stats, double value) { stats.damage.elemental.mainType = DamageElemental::ELEMENT_TYPE_FIRE; stats.damage.elemental.secondaryType = DamageElemental::ELEMENT_TYPE_ARCANE; }},
        {"THIS AURA GRANTS YOU FIRE DAMAGE BUFF", [&](Stats& stats, double value) { stats.damage.elemental.fire += Stacks::AURA_DAMAGE_BUFF * 50 / 100; }},
        {"BONUS FIRE DAMAGE FOR A SHORT DURATION.", [&](Stats& stats, double value) { stats.damage.elemental.fire += 50 / 100; }},
    };

    // Use a static boolean to prevent repeated modifications
    static bool statActionsIsInitialized = false;
    if (!statActionsIsInitialized) {
        for (const auto& offhand : DamageOffhand::getOffhandMap()) {
            statActions.insert(std::make_pair(offhand.second + " Damage", [&](Stats& stats, double value) { stats.damage.offhand.damage[offhand.first] += value / 100; }));
            statActions.insert(std::make_pair(offhand.second + " Damage Bonus", [&](Stats& stats, double value) { stats.damage.offhand.bonusDamage[offhand.first] += value / 100; }));
        }
        for (const auto& weapon : DamageWeapon::getWeaponMap()) {
            statActions.insert(std::make_pair(weapon.second + " Damage", [&](Stats& stats, double value) { stats.damage.weapon.damage[weapon.first] += value; }));
            statActions.insert(std::make_pair(weapon.second + " Critical Chance", [&](Stats& stats, double value) { stats.damage.weapon.criticalChance[weapon.first] += value / 100; }));
            statActions.insert(std::make_pair(weapon.second + " Critical Damage", [&](Stats& stats, double value) { stats.damage.weapon.criticalDamage[weapon.first] += value / 100; }));
        }
        const std::map<Stacks::STACK_TYPE, int>& recommendedStacks = Stacks::getRecommendedStacksMap();
        for (const auto& stack : Stacks::getStacksMap()) {
            statActions.insert(std::make_pair(stack.second, [&](Stats& stats, double value) { stats.stacks.increase(stack.first, recommendedStacks.at(stack.first)); }));
        }
        statActionsIsInitialized = true; // Ensure this block runs only once
    }

    if (statActions.count(statName)) {    // Check existence before using []
        if (verbose) std::cout << "\t" << statName << " " << value << std::endl;
        statActions[statName](stats, value);     // Safely call the function
    }
    else {
        Util::ResetConsoleColor();
        std::cerr << "\t\tStat not found: '" << statName << "'" << std::endl;
    }
}

std::vector<Stats> ItemParser::ParseStatsFromFile(const std::string& filename) {
    std::vector<Stats> stats;
    Stats stat;

    std::ifstream file(filename);
    std::string line;
    int itemColour = FOREGROUND_INTENSITY; // FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    if (verbose) {
        Util::ResetConsoleColor();
        std::cout << std::endl << "Parsing file: " << filename << std::endl;
    }
    int skipLines = 0;
    bool isTree = false;
    while (std::getline(file, line)) {
        {
            if (skipLines > 0) {
                skipLines--;
                continue;
            }
            if (line == "" && !isTree) {
                if (stat != Stats()) {
                    stats.push_back(stat);
                    stat = Stats();
                }
               
            }
            if (line == "type: cards") {
                return { ParseCardsFromFile(filename) };
            }
            if (line == "type: tree") {
                isTree = true;
                continue;
            }
            if (line == "type: purple") {
                if (verbose) itemColour = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                skipLines = 2;
                continue;
            }
            if (line == "type: gray") {
                if (verbose) itemColour = FOREGROUND_INTENSITY;
                skipLines = 2;
                continue;
            }
            if (line == "type: orange") {
                if (verbose) itemColour = FOREGROUND_RED | FOREGROUND_INTENSITY;
                skipLines = 2;
                continue;
            }
            if (line == "type: blue") {
                if (verbose) itemColour = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                skipLines = 2;
                continue;
            }
            if (line == "type: gold") {
                if (verbose) itemColour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                skipLines = 2;
                continue;
            }
        }

        if (verbose) Util::SetConsoleColor(itemColour);

        double value = 0;

        // Extract numeric value from line
         // Trim and clean up the remaining part of the line to get the potential stat name
        auto beforeTrim = line;
        line = Trim(line);

        // Find the closest matching stat name
        std::replace(line.begin(), line.end(), ',', '.');
        bool hasNumber = ExtractNumber(line, value);

        static std::vector<std::string> ignoreStats = {
            // item types
            "Body", "Helmet", "Gloves", "Pants", "Magery", "Amulet", "Bracer", "Ring", "Relic", "Shoes",
            // dragon
            "powerful yet friendly", "WHER ENEMIES DIiE THEIR CORPSE HAS A CHANCE TO", "EXPLODE DEALING FiRE DAMAGE",
            "YOuR DRAGONLING USES ITS MAGIC TO DIRECTLY", "PICK UP ALL LOOT BAGS",
            "sonus", "CHANCE TO SPAWN AN AURA NEAR YOU EnTERING", "AFTER A DASH CAUSE A FiRE EXPLOSION AND GAin",
            // fire beam
            "Projects an intense beam of concentrated fire", "forward Inflicts damage to enemies in its path", "creating a linear zone of destruction",
            "Fire BEAM AFTER A FEW SECONDS THE BEAM WiLL", "EXPAND LASTING LONGER AND DEALING", "SiGNIFICANTLY MORE DAMAGE",
            "Fire BEAM AFTER A FEW SECONDS THE BEAM WiLL",
            // spinning blade
            "around the caster Creates a mobile zone of", "cutting force damaging enemies who venture", "too close",
            // electric dragons
            "Manifests a ring of electric dragons around the", "caster Dragons orbit the user crackling with",
            "intense electrical energy Inflicts damage to", "foes who come into contact with the ethereal", "serpents r",
            // fossil
            "An ancient life source with a powerful force f",
            "Can be imbued with magical gems. You only", "need ONE fossil.",
            // unused
            " ", "Gain  MOVEMENT SPEED", "Procs", "Crafting Specks", "WHER KiLLING AN iMBUED BEETLE SHARE REWARD", "WIiTH ENTIRE FELLOWSHIP",
            "WHER KiLLING An ELITE ENEMY GaAin  TO sPawn", "ANOTHER ELITE", "burning core Damage",
            "GET  MOVEMENT SPEED PER MAX POTION SLOTS",
        };
        bool ignore = false;
        auto before = line;
        line.erase(std::remove_if(line.begin(), line.end(), ::ispunct), line.end());
        //line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        line = Trim(line);
//#pragma omp parallel for shared(ignore) num_threads(Util::getMaxThreads())
        for (int i = 0; i < ignoreStats.size(); ++i) {
            // If ignore is already true, skip further checks
            if (ignore) continue;
            if (LevenshteinDistance(ignoreStats[i], line) <= std::max<double>(MIN_DISTANCE, line.length() / 9)) {
                for (const auto& weapon : DamageWeapon::getWeaponMap()) {
                    if (LevenshteinDistance(ignoreStats[i], weapon.second) <= MIN_DISTANCE)
                        stat.damage.weapon.type = weapon.first;
                }
#pragma omp critical
                ignore = true;
            }
        }
        if (ignore) continue;

        if (line == "P ")
            line = "Armour"; // :(
        if (line == "| ")
            line = "Damage"; // :(

        line = before;
        line.erase(std::remove_if(line.begin(), line.end(), ::ispunct), line.end());
        //line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        line = Trim(line);

        std::string statName = FindClosestStatName(line);
        if (hasNumber) {
            if (!statName.empty()) {
                ApplyStat(stat, statName, value);
            }
        }
        else {
            for (const auto& stack : Stacks::getStacksMap()) {
                if (stack.second == statName)
                    ApplyStat(stat, statName);
            }
        }
    }

    if (verbose) Util::ResetConsoleColor();

    stats.push_back(stat);

    return stats;
}

Stats ItemParser::ParseCardsFromFile(const std::string& filename) {
    Stats stats;

    std::ifstream file(filename);
    std::string line;
    //std::cout << "Parsing file: " << filename << std::endl;

    std::map<std::string, int> cardListMap;
    const std::vector<Predictor::PredictedStats>& cardList = Predictor::getCardList();
    for (int i = 0; i < cardList.size(); ++i) {
        cardListMap.insert(std::make_pair(Predictor::statListStr(cardList[i]), i));
    }

    Predictor::resetParsedCards();

    while (std::getline(file, line)) {
        if (line == "type: cards") {
            continue;
        }

        // Skip empty lines and lines starting with '#' (comments)
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Find the first occurrence of '('
        size_t openParen = line.find('"');
        if (openParen == std::string::npos) {
            Util::ResetConsoleColor();
            std::cerr << "\t\tCan't find first \" when parsing line: " << line << std::endl;
            continue;
        }

        // Find the matching closing parenthesis
        size_t closeParen = line.find('")"', openParen + 1);
        if (closeParen == std::string::npos) {
            Util::ResetConsoleColor();
            std::cerr << "\t\tCan't find second \" when parsing line: " << line << std::endl;
            continue;
        }

        // Extract the attribute string
        std::string cardDef = line.substr(openParen + 1, closeParen - openParen - 1);
        if (cardListMap.count(cardDef) == 0) {
            Util::ResetConsoleColor();
            std::cerr << "\t\tCan't find card definition when parsing line: " << line << std::endl;
            continue;
        }


        int cardCount;
        std::istringstream iss(line.substr(closeParen + 1));
        if (!(iss >> cardCount)) {
            Util::ResetConsoleColor();
            std::cerr << "\t\tError convert value to int: " << line << std::endl;
            continue;
        }

        if (cardCount == 0)
            continue;

        if (cardCount > 3)
            cardCount = 6;

        if (verbose) Util::SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        const Predictor::PredictedStats& cardStats = cardList.at(cardListMap.at(cardDef));
        for (const Predictor::PredictedStat& cardStat : cardStats) {
            ApplyStat(stats, cardStat.first, cardStat.second * cardCount);
        }
        Predictor::parseCard(cardStats, cardCount);
    }

    if (verbose) Util::ResetConsoleColor();

    return stats;
}

// Utility function to convert std::string to std::wstring
std::wstring ItemParser::StringToWString(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}

// Utility function to convert std::wstring to std::string
std::string ItemParser::WStringToString(const std::wstring& wstr) {
    return std::string(wstr.begin(), wstr.end());
}

// Function to list all .txt files in a specified folder
std::vector<std::string> ItemParser::FindTxtFilesInFolder(const std::string& folderPath) {
    std::vector<std::string> txtFiles;
    std::wstring searchPath = StringToWString(folderPath) + L"\\*.txt"; // Convert search pattern to wide string

    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to open directory or no .txt files found." << std::endl;
        return txtFiles;
    }

    do {
        // Check if it's a file (not a directory)
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            txtFiles.push_back(WStringToString(findData.cFileName)); // Convert file name to std::string and add to the list
        }
    } while (FindNextFileW(hFind, &findData) != 0);

    FindClose(hFind); // Close the find handle
    return txtFiles;
}