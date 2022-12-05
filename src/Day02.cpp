#include "Day02.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Result.h"

static const int ScoreTablePart1[] = { 3, 0, 6, 6, 3, 0, 0, 6, 3 };
static const int ScoreTablePart2[] = { 3, 1, 2, 1, 2, 3, 2, 3, 1 };

bool parseLetter(char letter, int& outShape)
{
    switch (letter)
    {
    case 'A':
    case 'X':
        outShape = 0;
        return true;
    case 'B':
    case 'Y':
        outShape = 1;
        return true;
    case 'C':
    case 'Z':
        outShape = 2;
        return true;
    }

    std::cout << "invalid letter '" << letter << "'" << std::endl;
    return false;
}

int computePairScorePart1(std::pair<int, int> pair) {
    int shapeScore = pair.second + 1;
    int tableIndex = pair.first + pair.second * 3;
    int outcomeScore = ScoreTablePart1[tableIndex];
    return shapeScore + outcomeScore;
}

int computePairScorePart2(std::pair<int, int> pair) {
    int outcomeScore = pair.second * 3;
    int tableIndex = pair.first + pair.second * 3;
    int shapeScore = ScoreTablePart2[tableIndex];
    return shapeScore + outcomeScore;
}

bool Day02::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        if (line.size() >= 3) {
            int first, second;
            if (parseLetter(line[0], first) && parseLetter(line[2], second)) {
                strategyGuide.push_back(std::pair(first, second));
            }
            else {
                return false;
            }
        }
        else {
            std::cout << "invalid line '" << line << "'" << std::endl;
        }
    }

    return true;
}

Result Day02::runPart1() const
{
    // Sum the score of all pairs
    int score = 0;
    for (std::pair pair : strategyGuide) {
        score += computePairScorePart1(pair);
    }

    return score;
}

Result Day02::runPart2() const
{
    // Sum the score of all pairs, using the new scoring rule
    int score = 0;
    for (std::pair pair : strategyGuide) {
        score += computePairScorePart2(pair);
    }

    return score;
}
