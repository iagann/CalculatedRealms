# CalculatedRealms

CalculatedRealms is a console-based C++ application that provides various calculations and predictions for game builds, items, attributes, and more.
The program interacts with the user through keyboard inputs to perform specific tasks related to build optimization and evaluation.

## Current character build (items/tree/cards) definition
The program uses user items/tree/cards defined in the folder
```#define ITEMS_FOLDER "items/chaos3mini"```
check the files in that folder as example. 

It's recommended to use OCR software to get item stats from the game fast.

To generate tree stats, use this website: [Dwarver Realms passive tree planner](https://dwarvenleaderboar.com/tree). Just copy the stats to the file tree.txt, and don't forget to include "type: tree" as the first line of that file

## Features

- **Compare Items (`I`)**: Evaluates and compares different item combinations based on various metrics.
- **Predict Cards (`C`)**: Provides optimal predictions for cards based on reroll possibilities and stats.
- **Predict Attributes (`A`)**: Calculates the best attributes for given scenarios, optimizing for specific outcomes.
- **Explain Rating (`R`)**: Breaks down and explains the detailed rating of the current build.
- **Predict Tree (`T`)**: Suggests optimal nodes or paths within a skill tree to enhance the build.
- **Help (`H`)**: Displays the list of available commands and their descriptions.
- **Quit (`Q`)**: Exits the application.

## How to Use

1. **Run the Application**: Start the application from your terminal or preferred development environment.
2. **Interact with the Application**: Use the following keys to interact with the program:
   - `I` - Compare items.
   - `C` - Predict cards.
   - `A` - Predict attributes.
   - `R` - Explain the current build rating.
   - `T` - Predict the skill tree path.
   - `H` - Display help information.
   - `Q` - Quit the application.

