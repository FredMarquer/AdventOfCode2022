#include "StdAfx.h"

#include "Day01.h"

#include "Result.h"

void Day01::parseFile(std::ifstream& file)
{
    std::string line{};
    caloriesPerElves.push_back(0);
    while (std::getline(file, line)) {
        if (line.empty())
            caloriesPerElves.push_back(0);
        else
            caloriesPerElves.back() += std::stoi(line);
    }
}

Result Day01::runPart1() const
{
    return std::ranges::max(caloriesPerElves);
}

Result Day01::runPart2() const
{
    std::vector<int> vec = caloriesPerElves;
    std::nth_element(vec.begin(), vec.begin() + 3, vec.end(), std::greater{});
    return std::accumulate(vec.begin(), vec.begin() + 3, 0);
}

Result Day01::getExpectedResultPart1() const
{
    return 70374;
}

Result Day01::getExpectedResultPart2() const
{
    return 204610;
}
