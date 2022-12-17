#include "Day04.h"

#include <assert.h>
#include <fstream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "Result.h"
#include "Utils/Exception.h"
#include "Utils/Range.h"

void Day04::parseFile(std::ifstream& file)
{
    std::string line;
    std::regex regex("([0-9]*)-([0-9]*),([0-9]*)-([0-9]*)");
    std::smatch matches;
    while (std::getline(file, line)) {
        if (!std::regex_search(line, matches, regex))
            exception("no match found for line: {}", line);
        int firstMin = std::stoi(matches[1]);
        int firstMax = std::stoi(matches[2]) + 1;
        int secondMin = std::stoi(matches[3]);
        int secondMax = std::stoi(matches[4]) + 1;
        std::pair pair(Range(firstMin, firstMax), Range(secondMin, secondMax));
        pairs.push_back(pair);
    }
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

Result Day04::getExpectedResultPart1() const
{
    return 498;
}

Result Day04::getExpectedResultPart2() const
{
    return 859;
}
