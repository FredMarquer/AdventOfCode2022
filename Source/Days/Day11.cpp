#include "Day11.h"

#include <assert.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <string>
#include <string_view>

#include "Result.h"
#include "Utils/Log.h"
#include "Utils/Parsing.h"

bool tryParseItems(const std::string_view& line, std::vector<int64_t>& items)
{
    size_t separator = line.find_first_of(',');
    if (separator != std::string::npos) {
        int64_t item;
        if (!tryParse(line.substr(0, separator), item))
            return false;

        items.push_back(item);

        // Parse the next item recursively
        return tryParseItems(line.substr(separator + 2), items);
    }
    else {
        int64_t item;
        if (!tryParse(line, item))
            return false;

        items.push_back(item);
        return true;
    }
}

bool tryParseIntAtEnd(const std::string_view& line, size_t position, int64_t& outValue)
{
    return tryParse(line.substr(position), outValue);
}

bool tryParseIntAtEnd(const std::string_view& line, size_t position, size_t& outValue)
{
    return tryParse(line.substr(position), outValue);
}

bool tryParseInspectFunction(const std::string_view& line, ItemModifierFunction& outFunction)
{
    char operation = line[23];
    if (operation == '+') {
        // Add operation
        int64_t value;
        if (!tryParseIntAtEnd(line, 25, value))
            return false;
        outFunction = [value](int64_t& item) { item += value; };
    }
    else if (operation == '*') {
        if (line[25] == 'o') {
            // Pow2 operation
            outFunction = [](int64_t& item) { item *= item; };
        }
        else {
            // Mul operation
            int64_t value;
            if (!tryParseIntAtEnd(line, 25, value))
                return false;
            outFunction = [value](int64_t& item) { item *= value; };
        }
    }
    else {
        error("invalid operator: {}", operation);
        return false;
    }

    return true;
}

bool tryParseMonkey(std::ifstream& file, Monkey& outMonkey)
{
    assert(outMonkey.startingItems.size() == 0);

    std::string line;

    // Skip the first line
    if (!std::getline(file, line))
        return false;

    // Parse items
    if (!std::getline(file, line) || !tryParseItems(std::string_view(line).substr(18), outMonkey.startingItems))
        return false;

    // Parse inspectFunction
    if (!std::getline(file, line) || !tryParseInspectFunction(line, outMonkey.inspectFunction))
        return false;

    // Parse testFunction
    if (!std::getline(file, line) || !tryParseIntAtEnd(line, 21, outMonkey.testDivisor))
        return false;

    // Parse monkeyIndexIfTrue
    if (!std::getline(file, line) || !tryParseIntAtEnd(line, 29, outMonkey.monkeyIndexIfTrue))
        return false;

    // Parse monkeyIndexIfFalse
    if (!std::getline(file, line) || !tryParseIntAtEnd(line, 30, outMonkey.monkeyIndexIfFalse))
        return false;

    // Skip empty line
    std::getline(file, line);

    return true;
}

bool Day11::parseFile(std::ifstream& file)
{
    Monkey monkey;
    while (tryParseMonkey(file, monkey))
        monkeys.push_back(std::move(monkey));
    
    return true;
}

int64_t simulate(const std::vector<Monkey>& monkeys, int numberOfRound, const ItemModifierFunction& worryLevelManagementFunction)
{
    // Initialize the working data
    size_t monkeyCount = monkeys.size();
    std::vector<int64_t> inspectCountPerMonkey(monkeyCount);
    std::vector<std::vector<int64_t>> itemsPerMonkey(monkeyCount);
    for (size_t monkeyIndex = 0; monkeyIndex < monkeyCount; ++monkeyIndex)
        itemsPerMonkey[monkeyIndex] = monkeys[monkeyIndex].startingItems;

    // Simulate X rounds
    for (int round = 0; round < numberOfRound; ++round) {
        for (size_t monkeyIndex = 0; monkeyIndex < monkeyCount; ++monkeyIndex) {
            const Monkey& monkey = monkeys[monkeyIndex];
            std::vector<int64_t>& items = itemsPerMonkey[monkeyIndex];
            int64_t& inspectCount = inspectCountPerMonkey[monkeyIndex];

            for (int64_t item : items) {
                // Inspect
                monkey.inspectFunction(item);
                ++inspectCount;

                // Manage the worry level 
                worryLevelManagementFunction(item);

                // Throw the item
                if (item % monkey.testDivisor == 0)
                    itemsPerMonkey[monkey.monkeyIndexIfTrue].push_back(item);
                else
                    itemsPerMonkey[monkey.monkeyIndexIfFalse].push_back(item);
            }

            items.clear();
        }
    }

    // Find the 2 highest inspect count 
    std::vector<int64_t> top2(2);
    for (int64_t inspectCount : inspectCountPerMonkey) {
        for (size_t i = 0; i < 2; ++i) {
            if (inspectCount > top2[i]) {
                top2.insert(top2.begin() + i, inspectCount);
                top2.pop_back();
                break;
            }
        }
    }

    assert(top2.size() == 2);
    return top2[0] * top2[1];
}

Result Day11::runPart1() const
{
    ItemModifierFunction worryLevelManagementFunction = [](int64_t& item) { item /= 3; };

    return simulate(monkeys, 20, worryLevelManagementFunction);
}

Result Day11::runPart2() const
{
    // Find the common denominator
    int64_t commonDenominator = 1;
    for (size_t monkeyIndex = 0; monkeyIndex < monkeys.size(); ++monkeyIndex)
        commonDenominator *= monkeys[monkeyIndex].testDivisor;

    ItemModifierFunction worryLevelManagementFunction = [commonDenominator](int64_t& item) { item %= commonDenominator; };

    return simulate(monkeys, 10000, worryLevelManagementFunction);
}

bool Day11::tryGetExpectedResultPart1(Result& outResult) const
{
    outResult = 119715;
    return true;
}

bool Day11::tryGetExpectedResultPart2(Result& outResult) const
{
    outResult = 18085004878;
    return true;
}
