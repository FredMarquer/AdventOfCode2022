#include "Day02.h"

#include <array>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "Result.h"
#include "Utils/Log.h"

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

    error("invalid letter: {}", letter);
    return false;
}

static const int ScoreTablePart1[] = { 3, 0, 6, 6, 3, 0, 0, 6, 3 };

int computePairScorePart1(std::pair<int, int> pair) {
    int shapeScore = pair.second + 1;
    size_t tableIndex = pair.first + pair.second * 3;
    int outcomeScore = ScoreTablePart1[tableIndex];
    return shapeScore + outcomeScore;
}

static const int ScoreTablePart2[] = { 3, 1, 2, 1, 2, 3, 2, 3, 1 };

int computePairScorePart2(std::pair<int, int> pair) {
    int outcomeScore = pair.second * 3;
    size_t tableIndex = pair.first + pair.second * 3;
    int shapeScore = ScoreTablePart2[tableIndex];
    return shapeScore + outcomeScore;
}

bool Day02::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        if (line.size() >= 3) {
            int first, second;
            if (parseLetter(line[0], first) && parseLetter(line[2], second))
                strategyGuide.push_back(std::pair(first, second));
            else
                return false;
        }
        else
            error("invalid line: {}", line);
    }

    return true;
}

Result Day02::runPart1() const
{
    // Sum the score of all pairs
    int score = 0;
    for (std::pair pair : strategyGuide)
        score += computePairScorePart1(pair);

    return score;
}

Result Day02::runPart2() const
{
    // Sum the score of all pairs, using the new scoring rule
    int score = 0;
    for (std::pair pair : strategyGuide)
        score += computePairScorePart2(pair);

    return score;
}

bool Day02::tryGetExpectedResultPart1(Result& outResult) const
{
    outResult = 11873;
    return true;
}

bool Day02::tryGetExpectedResultPart2(Result& outResult) const
{
    outResult = 12014;
    return true;
}
