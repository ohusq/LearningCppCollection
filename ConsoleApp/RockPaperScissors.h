#pragma once
#include <random> // RNG
#include <iostream> // cout
#include <map> // Dictionary

int playRockPaperScissors() {
    // Rock > Scissors [Rock 1]
    // Scissors > Paper [Paper 2]
    // Paper > Rock [Scissors 3]

    const int min = 1;
    const int max = 3;

    std::random_device rd;      // Only used once to initialise (seed) engine
    std::mt19937 rng(rd());     // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(min, max); // Guaranteed unbiased

    std::map<int, std::string> values; // Dictionary to compare
    values[1] = "Rock";
    values[2] = "Paper";
    values[3] = "Scissors";

    const int DEFENSE = uni(rng); // Generates a random int between 1-3

    int playerChoice;
    std::cout << "Please choose an attack." << std::endl;
    std::cout << "1. Rock" << std::endl;
    std::cout << "2. Paper" << std::endl;
    std::cout << "3. Scissors" << std::endl;

    std::cout << "Enter your choice (1-3): ";
    std::cin >> playerChoice;

    if (playerChoice < 1 || playerChoice > 3) { // Check if the choice is valid
        std::cout << "Invalid choice. Please choose between 1 and 3." << std::endl;
        return 0;
    }

    std::cout << "You chose: " << values[playerChoice] << std::endl;
    std::cout << "Computer chose: " << values[DEFENSE] << std::endl;

    // Determine the winner
    if (playerChoice == DEFENSE) {
        std::cout << "It's a tie!" << std::endl;
    }
    else if ((playerChoice % 3) + 1 == DEFENSE) {
        std::cout << "Computer wins!" << std::endl;
    }
    else {
        std::cout << "You win!" << std::endl;
    }

    return 0;
}
