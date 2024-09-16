#include "Calculator.h"
#include "ItemParser.h"
#include "Predictor.h"

#include <iostream>
#include <conio.h> // For _kbhit() and _getch()

#define ITEMS_FOLDER "items/chaos3"

int main()
{
    setlocale(LC_ALL, "en_US.UTF-8");

    char key = 'H';
    bool firstLaunch = true;
    while (1) {
        if (!firstLaunch)
            key = _getch();
        else
            firstLaunch = false;

        Calculator calculator;
        //Calculator::setWeaponType(DamageWeapon::WEAPON_TYPE_MAGERY);
        calculator.setOffhandName(DamageOffhand::OFFHAND_TYPE_DRAGON_FLAME);

        calculator.setInit(Stats::init());

        std::map<std::string, std::vector<Stats>> stats;

        auto files = ItemParser::FindTxtFilesInFolder(ITEMS_FOLDER);
        for (const auto& file : files) {
            stats.insert(std::make_pair(file, ItemParser::ParseStatsFromFile(ITEMS_FOLDER + std::string("/") + file)));
        }
        calculator.setStats(stats);

        switch (key) {
        case 'c':
            std::cout << "C key pressed\n";
            Predictor::predictCards(calculator);
            break;
        case 'i':
        //case 'ш':
            std::cout << "I key pressed\n";
            calculator.getBestItemCombo(true);
            calculator.getRatingSimple();
            break;
        case 'a':
        //case 'ф':
            std::cout << "A key pressed\n";
            Predictor::predictAttributes(calculator);
            break;
        case 't':
        //case 'е':
            std::cout << "T key pressed\n";
            Predictor::predictTree(calculator);
            break;
        case 'r':
        //case 'к':
            std::cout << "R key pressed\n";
            calculator.getBestItemCombo();
            calculator.verbose = 1;
            calculator.getRating();
            calculator.verbose = 2;
            break;
        case 's':
        //case 'ы':
            std::cout << "P key pressed\n";
            ItemParser::verbose = 1;
            for (const auto& file : files) {
                ItemParser::ParseStatsFromFile(ITEMS_FOLDER + std::string("/") + file);
            }
            ItemParser::verbose = 0;
            break;
        case 'q':
        //case 'й':
            std::cout << "Q key pressed. Quitting...\n";
            // Perform action for 'q'
            return 0; // Exit the loop and program
        default:
            std::cout << "Supported keys:\n"
                << "\tI - compare items\n"
                << "\tC - predict cards\n"
                << "\tA - predict attributes\n"
                << "\tR - explain rating\n"
                << "\tT - predict tree\n"
                << "\tS - show parsed stats\n"
                << "\tH - help\n"
                << "\tQ - quit\n";
            break;
        }

        std::cout << "\n";
    }

    return 0;
}