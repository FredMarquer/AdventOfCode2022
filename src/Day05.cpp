#include "Day05.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "Result.h"

void applyInstructionCrateMover9000(const Instruction& instruction, std::vector<std::vector<char>>& stacks)
{
    std::vector<char>& fromStack = stacks[instruction.fromStackIndex];
    std::vector<char>& toStack = stacks[instruction.toStackIndex];

    for (int i = 0; i < instruction.numberOfBoxes; ++i) {
        toStack.push_back(fromStack.back());
        fromStack.pop_back();
    }
}

void applyInstructionCrateMover9001(const Instruction& instruction, std::vector<std::vector<char>>& stacks)
{
    std::vector<char>& fromStack = stacks[instruction.fromStackIndex];
    std::vector<char>& toStack = stacks[instruction.toStackIndex];

    int fromStackSize = fromStack.size();
    for (int boxIndex = fromStackSize - instruction.numberOfBoxes; boxIndex < fromStackSize; ++boxIndex) {
        toStack.push_back(fromStack[boxIndex]);
    }

    for (int i = 0; i < instruction.numberOfBoxes; ++i) {
        fromStack.pop_back();
    }
}

std::string result(const std::vector<std::vector<char>>& stacks)
{
    std::string result;
    for (const auto& stack : stacks) {
        result.push_back(stack.back());
    }

    return result;
}

bool Day05::parseFile(std::ifstream& file)
{
    stacks.resize(9);

    // Parse the box stacks
    std::string line;
    while (std::getline(file, line)) {
        if (line[1] == '1') {
            break;
        }
        for (int i = 0; i < 9; ++i) {
            int pos = i * 4 + 1;
            char box = line[pos];
            if (box != ' ') {
                std::vector<char>& stack = stacks[i];
                stack.insert(stack.begin(), box);
            }
        }
    }

    // Skip the blank line
    std::getline(file, line);
    assert(line.empty());

    // Parse the instruction
    std::regex regex("move ([0-9]*) from ([0-9]*) to ([0-9]*)");
    std::smatch matches;
    while (std::getline(file, line)) {
        if (std::regex_search(line, matches, regex)) {
            int numberOfBoxes = std::stoi(matches[1]);
            int fromStackIndex = std::stoi(matches[2]) - 1;
            int toStackIndex = std::stoi(matches[3]) - 1;
            instructions.push_back(Instruction(numberOfBoxes, fromStackIndex, toStackIndex));
        }
        else {
            std::cout << "no match found for line : " << line << std::endl;
            return false;
        }
    }

    return true;
}

Result Day05::runPart1() const
{
    // Copy the stacks
    std::vector workingStacks = stacks;

    // Process the instructions
    for (const Instruction& instruction : instructions) {
        applyInstructionCrateMover9000(instruction, workingStacks);
    }

    return result(workingStacks);
}

Result Day05::runPart2() const
{
    // Copy the stacks
    std::vector workingStacks = stacks;

    // Process the instructions
    for (const Instruction& instruction : instructions) {
        applyInstructionCrateMover9001(instruction, workingStacks);
    }

    return result(workingStacks);
}