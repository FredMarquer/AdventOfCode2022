#include "StdAfx.h"

#include "Day21.h"

#include "Result.h"
#include "Utils/Exception.h"
#include "Utils/Log.h"
#include "Utils/Parsing.h"

namespace
{
    static std::regex operationRegex("([a-z]{4}) . ([a-z]{4})");
    static std::string root = "root";
    static std::string humn = "humn";

    Day21::OpCodes charToOpCode(char c)
    {
        switch (c) {
        case '+': return Day21::OpCodes::Add;
        case '-': return Day21::OpCodes::Sub;
        case '*': return Day21::OpCodes::Mul;
        case '/': return Day21::OpCodes::Div;
        default:
            exception("invalid character: {}", c);
        }
    }

    void parseLine(const std::string& line, std::string& outName, Day21::Monkey& outMonkey)
    {
        outName = line.substr(0, 4);

        if (line[6] >= '0' && line[6] <= '9') {
            int value;
            parse(line.substr(6), value);
            outMonkey = value;
        }
        else {
            std::smatch matches;
            if (!std::regex_search(line, matches, operationRegex))
                exception("no match found for line: {}", line);
            outMonkey = Day21::Operation(charToOpCode(line[11]), matches[1], matches[2]);
        }
    }

    void setMonkeyParent(const std::string& monkeyName, const std::string& parentName, const std::unordered_map<std::string, int>& nameToIndex, std::vector<Day21::Monkey>& monkeys)
    {
        // Find monkey
        auto it = nameToIndex.find(monkeyName);
        if (it == nameToIndex.end()) {
            error("monkey not found: {}", monkeyName);
            return;
        }

        // Set parent
        int monkeyIndex = (*it).second;
        Day21::Monkey& monkey = monkeys[monkeyIndex];
        monkey.parent = parentName;

        // Set children's parent
        if (std::holds_alternative<Day21::Operation>(monkey.variant)) {
            const Day21::Operation& operation = std::get<Day21::Operation>(monkey.variant);
            setMonkeyParent(operation.left, monkeyName, nameToIndex, monkeys);
            setMonkeyParent(operation.right, monkeyName, nameToIndex, monkeys);
        }
    }

    int64_t evaluateMonkey(const std::string& monkeyName, const std::unordered_map<std::string, int>& nameToIndex, const std::vector<Day21::Monkey>& monkeys)
    {
        // Find monkey
        auto it = nameToIndex.find(monkeyName);
        if (it == nameToIndex.end()) {
            error("monkey not found: {}", monkeyName);
            return 0;
        }

        int monkeyIndex = (*it).second;
        const Day21::Monkey& monkey = monkeys[monkeyIndex];

        // Evaluate monkey
        if (std::holds_alternative<int>(monkey.variant))
            return std::get<int>(monkey.variant);
        else {
            const Day21::Operation& operation = std::get<Day21::Operation>(monkey.variant);

            int64_t leftValue = evaluateMonkey(operation.left, nameToIndex, monkeys);
            int64_t rightValue = evaluateMonkey(operation.right, nameToIndex, monkeys);

            switch (operation.opCode) {
            case Day21::OpCodes::Add: return leftValue + rightValue;
            case Day21::OpCodes::Sub: return leftValue - rightValue;
            case Day21::OpCodes::Mul: return leftValue * rightValue;
            case Day21::OpCodes::Div: return leftValue / rightValue;
            default:
                error("invalid op code");
                return 0;
            }
        }
    }

    int64_t findParentMissingNumber(const std::string& monkeyName, const std::string& fromMonkeyName, const std::unordered_map<std::string, int>& nameToIndex, const std::vector<Day21::Monkey>& monkeys)
    {
        // Find monkey index
        auto it = nameToIndex.find(monkeyName);
        if (it == nameToIndex.end()) {
            error("monkey not found: {}", monkeyName);
            return 0;
        }

        int monkeyIndex = (*it).second;
        const Day21::Monkey& monkey = monkeys[monkeyIndex];

        if (!std::holds_alternative<Day21::Operation>(monkey.variant)) {
            error("monkey '{}' is not an operation", monkeyName);
            return 0;
        }

        // Find the expected result of the operation
        int64_t total = 0;
        if (!monkey.parent.empty())
            total = findParentMissingNumber(monkey.parent, monkeyName, nameToIndex, monkeys);

        const Day21::Operation& operation = std::get<Day21::Operation>(monkey.variant);

        // Evaluate the other child
        int64_t other;
        bool otherIsLeft = operation.right == fromMonkeyName;
        if (otherIsLeft)
            other = evaluateMonkey(operation.left, nameToIndex, monkeys);
        else
            other = evaluateMonkey(operation.right, nameToIndex, monkeys);

        // Compute the missing number from the expected result and the other child number
        switch (operation.opCode) {
        case Day21::OpCodes::Add: return total - other;
        case Day21::OpCodes::Sub: return otherIsLeft ? other - total : total + other;
        case Day21::OpCodes::Mul: return total / other;
        case Day21::OpCodes::Div: return otherIsLeft ? other / total : total * other;
        case Day21::OpCodes::Equ: return other;
        default:
            error("invalid op code");
            return 0;
        }
    }
}

void Day21::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        std::string name;
        Monkey monkey;
        parseLine(line, name, monkey);
        nameToIndex.emplace(std::move(name), (int)monkeys.size());
        monkeys.push_back(std::move(monkey));
    }

    setMonkeyParent(root, "", nameToIndex, monkeys);
}

Result Day21::runPart1() const
{
    return evaluateMonkey(root, nameToIndex, monkeys);
}

Result Day21::runPart2() const
{
    std::vector<Day21::Monkey> workingMonkeys = monkeys;

    // Find the root monkey index
    auto rootIt = nameToIndex.find(root);
    if (rootIt == nameToIndex.end()) {
        error("root not found");
        return 0;
    }

    int rootIndex = (*rootIt).second;
    Day21::Monkey& rootMonkey = workingMonkeys[rootIndex];

    if (!std::holds_alternative<Day21::Operation>(rootMonkey.variant)) {
        error("root monkey is not an operation");
        return 0;
    }

    // Change root operation to Equ
    std::get<Day21::Operation>(rootMonkey.variant).opCode = OpCodes::Equ;

    // Find monkey index
    auto humnIt = nameToIndex.find(humn);
    if (humnIt == nameToIndex.end()) {
        error("humn not found");
        return 0;
    }

    // Find humn number
    int humnIndex = (*humnIt).second;
    const Day21::Monkey& humnMonkey = monkeys[humnIndex];
    return findParentMissingNumber(humnMonkey.parent, humn, nameToIndex, workingMonkeys);
}

Result Day21::getExpectedResultPart1() const
{
    return 84244467642604;
}

Result Day21::getExpectedResultPart2() const
{
    return 3759569926192;
}
