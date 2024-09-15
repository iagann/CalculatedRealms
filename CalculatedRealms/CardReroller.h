#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric> // For accumulate
#include <cmath>   // For pow

class CardReroller {
    // Struct to hold card information
    struct Card {
        double percentage;   // Original percentage from your data
        double probability;  // Normalized probability
        int value;           // Assigned numerical value
        std::string name;    // Card name and attributes
    };

public:
    static void printStrategy(const std::vector<std::pair<double, std::string>>& cardValues, int rerolls) {
        int N = cardValues.size(); // Total number of cards
        const int n = 4;                 // Number of cards drawn each time

        // Sort card values in descending order
        //std::sort(cardValues.begin(), cardValues.end(), std::greater<double>());

        // Compute average card value
        double sumValues = 0.0;
        for (auto v : cardValues) {
            sumValues += v.first;
        }
        double avgValue = sumValues / N;

        // Initialize expected values
        std::vector<double> E_reroll(rerolls + 1, avgValue); // Expected value when rerolling
        std::vector<int> optimalK(rerolls + 1, N);           // Optimal K for each M

        // For M = 0 (no rerolls), expected value is avgValue
        E_reroll[0] = avgValue;

        // Iterate over M from 1 to 5
        for (int m = 1; m <= rerolls; ++m) {
            double maxExpectedValue = 0.0;
            int bestK = N;

            // Try different K values
            for (int K = 1; K <= N; ++K) {
                // Compute probability of drawing at least one top K card
                double p_none = 1.0;
                for (int i = 0; i < n; ++i) {
                    p_none *= static_cast<double>(N - K - i) / (N - i);
                }
                double p_at_least_one = 1.0 - p_none;

                // Compute expected value if accepting now
                double sumTopK = 0.0;
                for (int i = 0; i < K; ++i) {
                    sumTopK += cardValues[i].first;
                }
                double E_accept = sumTopK / K;

                // Compute total expected value
                double E_total = p_at_least_one * E_accept + p_none * E_reroll[m - 1];

                // Update if this K gives a higher expected value
                if (E_total > maxExpectedValue) {
                    maxExpectedValue = E_total;
                    bestK = K;
                }
            }

            // Store the best K and update E_reroll for this M
            optimalK[m] = bestK;
            E_reroll[m] = maxExpectedValue;
        }

        // Output the optimal K for each M
        for (int m = rerolls; m >= 1; --m) {
            std::cout << "Optimal K for " << m << " rerolls left: " << optimalK[m] << std::endl;
            //std::cout << "Expected value: " << E_reroll[m] << std::endl;
        }
    }
};