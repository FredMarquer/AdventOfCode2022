#include "Day01.h"

#include <assert.h>
#include <fstream>
#include <string>
#include <vector>

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

bool Day01::tryGetExpectedResultPart1(Result& outResult) const
{
    outResult = 70374;
    return true;
}

bool Day01::tryGetExpectedResultPart2(Result& outResult) const
{
    outResult = 204610;
    return true;
}
