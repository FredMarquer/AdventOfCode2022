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

    int computePairScorePart1(int first, int second)
    {
        static constexpr int ScoreTable[] { 3, 0, 6, 6, 3, 0, 0, 6, 3 };

        int shapeScore = second + 1;
        int tableIndex = first + second * 3;
        int outcomeScore = ScoreTable[tableIndex];
        return shapeScore + outcomeScore;
    }

    int computePairScorePart2(int first, int second)
    {
        static constexpr int ScoreTable[] { 3, 1, 2, 1, 2, 3, 2, 3, 1 };

        int outcomeScore = second * 3;
        int tableIndex = first + second * 3;
        int shapeScore = ScoreTable[tableIndex];
        return shapeScore + outcomeScore;
    }

    int simulate(const std::vector<std::pair<int, int>>& strategyGuide, std::function<int(int, int)> computePairScore)
    {
        // Precompute all possible pair scores
        std::array<std::array<int, 3>, 3> pairScores;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j)
                pairScores[i][j] = computePairScore(i, j);
        }

        // Sum the score of all pairs in the strategy guide
        int score = 0;
        for (std::pair pair : strategyGuide)
            score += pairScores[pair.first][pair.second];

        return score;
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
    return simulate(strategyGuide, computePairScorePart1);
}

Result Day02::runPart2() const
{
    return simulate(strategyGuide, computePairScorePart2);
}

Result Day02::getExpectedResultPart1() const
{
    return 11873;
}

Result Day02::getExpectedResultPart2() const
{
    return 12014;
}
