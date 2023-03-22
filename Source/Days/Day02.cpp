#include "StdAfx.h"

#include "Day02.h"

#include "Result.h"
#include "Utils/Exception.h"

namespace
{
    int parseLetter(char letter)
    {
        switch (letter) {
        case 'A':
        case 'X':
            return 0;
        case 'B':
        case 'Y':
            return 1;
        case 'C':
        case 'Z':
            return 2;
        }

        exception("invalid letter: {}", letter);
    }

    int computePairScorePart1(std::pair<int, int> pair)
    {
        static constexpr int ScoreTable[] = { 3, 0, 6, 6, 3, 0, 0, 6, 3 };

        int shapeScore = pair.second + 1;
        int tableIndex = pair.first + pair.second * 3;
        int outcomeScore = ScoreTable[tableIndex];
        return shapeScore + outcomeScore;
    }

    int computePairScorePart2(std::pair<int, int> pair)
    {
        static constexpr int ScoreTable[] = { 3, 1, 2, 1, 2, 3, 2, 3, 1 };

        int outcomeScore = pair.second * 3;
        int tableIndex = pair.first + pair.second * 3;
        int shapeScore = ScoreTable[tableIndex];
        return shapeScore + outcomeScore;
    }
}

void Day02::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        if (line.size() < 3)
            exception("invalid line: {}", line);
        int first = parseLetter(line[0]);
        int second = parseLetter(line[2]);
        strategyGuide.push_back(std::pair(first, second));
    }
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

Result Day02::getExpectedResultPart1() const
{
    return 11873;
}

Result Day02::getExpectedResultPart2() const
{
    return 12014;
}
