#include "StdAfx.h"

#include "Day04.h"

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
        pairs.emplace_back(Range(firstMin, firstMax), Range(secondMin, secondMax));
    }
}

Result Day04::runPart1() const
{
    // Count the number of pairs where one range contains the other
    return std::ranges::count_if(pairs,
        [](const auto& pair) {
            return pair.first.contains(pair.second)
                || pair.second.contains(pair.first);
        });
}

Result Day04::runPart2() const
{
    // Count the number of overlapping pairs
    return std::ranges::count_if(pairs,
        [](const auto& pair) {
            return pair.first.overlap(pair.second);
        });
}

Result Day04::getExpectedResultPart1() const
{
    return 498;
}

Result Day04::getExpectedResultPart2() const
{
    return 859;
}
