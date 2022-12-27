#include "StdAfx.h"

#include "Day01.h"

#include "Result.h"

void Day01::parseFile(std::ifstream& file)
{
    int calories = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            caloriesPerElves.push_back(calories);
            calories = 0;
        }
        else
            calories += std::stoi(line);
    }
}

Result Day01::runPart1() const
{
    // Find the max calories value
    int maxCalories = 0;
    for (int calories : caloriesPerElves) {
        if (calories > maxCalories)
            maxCalories = calories;
    }

    return maxCalories;
}

Result Day01::runPart2() const
{
    // Find the top 3 max calories values
    std::vector<int> top3(3);
    for (int calories : caloriesPerElves) {
        for (size_t i = 0; i < 3; ++i) {
            if (calories > top3[i]) {
                top3.insert(top3.begin() + i, calories);
                top3.pop_back();
                break;
            }
        }
    }

    assert(top3.size() == 3);

    // Sum the top 3
    int sum = 0;
    for (int result : top3)
        sum += result;

    return sum;
}

Result Day01::getExpectedResultPart1() const
{
    return 70374;
}

Result Day01::getExpectedResultPart2() const
{
    return 204610;
}
