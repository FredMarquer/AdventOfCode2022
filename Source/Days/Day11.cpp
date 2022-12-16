#include "Day11.h"

#include <assert.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <string>
#include <string_view>

#include "Result.h"
#include "Utils/Exception.h"
#include "Utils/Parsing.h"

void parseItems(const std::string_view& line, std::vector<int64_t>& items)
{
    size_t separator = line.find_first_of(',');
    if (separator != std::string::npos) {
        int64_t item;
        parse(line.substr(0, separator), item);
        items.push_back(item);

        // Parse the next item recursively
        parseItems(line.substr(separator + 2), items);
    }
    else {
        int64_t item;
        parse(line, item);
        items.push_back(item);
    }
}

void parseIntAtEnd(const std::string_view& line, size_t position, int64_t& outValue)
{
    parse(line.substr(position), outValue);
}

void parseIntAtEnd(const std::string_view& line, size_t position, size_t& outValue)
{
    parse(line.substr(position), outValue);
}

void parseInspectFunction(const std::string_view& line, Day11::ItemModifierFunction& outFunction)
{
    char operation = line[23];
    if (operation == '+') {
        // Add operation
        int64_t value;
        parseIntAtEnd(line, 25, value);
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
            parseIntAtEnd(line, 25, value);
            outFunction = [value](int64_t& item) { item *= value; };
        }
    }
    else
        exception("invalid operator: {}", operation);
}

bool tryParseMonkey(std::ifstream& file, Day11::Monkey& outMonkey)
{
    assert(outMonkey.startingItems.size() == 0);

    std::string line;

    // Skip the first line
    if (!std::getline(file, line))
        return false;

    // Parse items
    if (!std::getline(file, line))
        exception("fail to get the 'items' line");
    parseItems(std::string_view(line).substr(18), outMonkey.startingItems);

    // Parse inspectFunction
    if (!std::getline(file, line))
        exception("fail to get the 'inspect function' line");
    parseInspectFunction(line, outMonkey.inspectFunction);

    // Parse testFunction
    if (!std::getline(file, line))
        exception("fail to get the 'test divider' line");
    parseIntAtEnd(line, 21, outMonkey.testDivisor);

    // Parse monkeyIndexIfTrue
    if (!std::getline(file, line))
        exception("fail to get the 'index if true' line");
    parseIntAtEnd(line, 29, outMonkey.monkeyIndexIfTrue);

    // Parse monkeyIndexIfFalse
    if (!std::getline(file, line))
        exception("fail to get the 'index if false' line");
    parseIntAtEnd(line, 30, outMonkey.monkeyIndexIfFalse);

    // Skip empty line
    std::getline(file, line);

    return true;
}

void Day11::parseFile(std::ifstream& file)
{
    Monkey monkey;
    while (tryParseMonkey(file, monkey))
        monkeys.push_back(std::move(monkey));
}

int64_t simulate(const std::vector<Day11::Monkey>& monkeys, int numberOfRound, const Day11::ItemModifierFunction& worryLevelManagementFunction)
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
            const Day11::Monkey& monkey = monkeys[monkeyIndex];
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
