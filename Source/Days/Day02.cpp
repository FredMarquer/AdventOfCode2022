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

    consteval std::array<std::array<int, 3>, 3> computePairScores(auto computePairScore)
    {
        // Precompute all possible pair scores
        std::array<std::array<int, 3>, 3> pairScores{};
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j)
                pairScores[i][j] = computePairScore(i, j);
        }

        return pairScores;
    }

    int simulate(const std::vector<std::pair<int, int>>& strategyGuide, const std::array<std::array<int, 3>, 3>& pairScores)
    {
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
    constexpr std::array<std::array<int, 3>, 3> pairScores = computePairScores([](int first, int second){
        constexpr int scoreTable[]{ 3, 0, 6, 6, 3, 0, 0, 6, 3 };
        int shapeScore = second + 1;
        int tableIndex = first + second * 3;
        int outcomeScore = scoreTable[tableIndex];
        return shapeScore + outcomeScore;
    });

    return simulate(strategyGuide, pairScores);
}

Result Day02::runPart2() const
{
    constexpr std::array<std::array<int, 3>, 3> pairScores = computePairScores([](int first, int second) {
        constexpr int scoreTable[]{ 3, 1, 2, 1, 2, 3, 2, 3, 1 };
        int outcomeScore = second * 3;
        int tableIndex = first + second * 3;
        int shapeScore = scoreTable[tableIndex];
        return shapeScore + outcomeScore;
    });

    return simulate(strategyGuide, pairScores);
}

Result Day02::getExpectedResultPart1() const
{
    return 11873;
}

Result Day02::getExpectedResultPart2() const
{
    return 12014;
}
