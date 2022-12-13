#include "Day04.h"

#include <assert.h>
#include <fstream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "Log.h"
#include "Result.h"

bool Day04::parseFile(std::ifstream& file)
{
    std::string line;
    std::regex regex("([0-9]*)-([0-9]*),([0-9]*)-([0-9]*)");
    std::smatch matches;
    while (std::getline(file, line)) {
        if (std::regex_search(line, matches, regex)) {
            int firstMin = std::stoi(matches[1]);
            int firstMax = std::stoi(matches[2]);
            int secondMin = std::stoi(matches[3]);
            int secondMax = std::stoi(matches[4]);
            std::pair pair(Range(firstMin, firstMax), Range(secondMin, secondMax));
            pairs.push_back(pair);
        }
        else {
            error("no match found for line: {}", line);
            return false;
        }
    }

    return true;
}

Result Day04::runPart1() const
{
    // Count the number of pairs where one range contains the other
    int containsCount = 0;
    for (const auto& pair : pairs) {
        if (pair.first.contains(pair.second) ||
            pair.second.contains(pair.first)) {
            ++containsCount;
        }
    }

    return containsCount;
}

Result Day04::runPart2() const
{
    // Count the number of overlapping pairs
    int overlapCount = 0;
    for (const auto& pair : pairs) {
        if (pair.first.overlap(pair.second))
            ++overlapCount;
    }

    return overlapCount;
}

bool Day04::tryGetExpectedResultPart1(Result& outResult) const
{
    outResult = 498;
    return true;
}

bool Day04::tryGetExpectedResultPart2(Result& outResult) const
{
    outResult = 859;
    return true;
}
