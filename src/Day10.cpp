#include "Day10.h"

#include <assert.h>
#include <charconv>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool tryParseInstruction(const std::string_view& line, InstructionD10& outInstruction)
{
    // Parse the op code first
    std::string_view opCodeView = line.substr(0, 4);
    if (opCodeView == "noop") {
        outInstruction = InstructionD10(OpCodes::Noop);
    }
    else if (opCodeView == "addx") {
        // Parse the operand
        int operand = 0;
        std::string_view operandView = line.substr(5);
        auto result = std::from_chars(operandView.data(), operandView.data() + operandView.size(), operand);
        if (result.ec != std::errc()) {
            std::cout << "invalid operand: " << operandView << std::endl;
            return false;
        }

        outInstruction = InstructionD10(OpCodes::Addx, operand);
    }
    else {
        std::cout << "invalid line: " << line << std::endl;
        return false;
    }

    return true;
}

bool Day10::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        InstructionD10 instruction;
        if (tryParseInstruction(line, instruction))
            instructions.push_back(instruction);
        else
            return false;
    }

    return true;
}

class Sampler
{
public:
    virtual void sample(int cycle, int x) = 0;
};

class SamplerPart1 : public Sampler
{
private:
    int result = 0;

public:
    int getResult() { return result; }

    void sample(int cycle, int x) override
    {
        bool needToSampleSingalStrength = ((cycle + 20) % 40 == 0) && cycle <= 220;
        if (needToSampleSingalStrength) {
            int singalStrength = cycle * x;
            result += singalStrength;
        }
    }
};

class SamplerPart2 : public Sampler
{
private:
    std::vector<bool> pixels;

public:
    std::string getResult()
    {
        assert(pixels.size() == 240);

        // Convert the pixels into a string
        std::string result;
        result.push_back('\n');
        for (int y = 0; y < 6; ++y) {
            for (int x = 0; x < 40; ++x) {
                int index = x + y * 40;
                char c = pixels[index] ? '#' : '.';
                result.push_back(c);
            }

            if (y < 5)
                result.push_back('\n');
        }

        return result;
    }

    void sample(int cycle, int x) override
    {
        int position = (cycle - 1) % 40;
        bool pixel = position <= x + 1 && position >= x - 1;
        pixels.push_back(pixel);
    }
};

void runInstructions(const std::vector<InstructionD10>& instructions, Sampler* sampler)
{
    assert(sampler != nullptr);

    int cycle = 0;
    int x = 1;

    int instructionIndex = 0;
    InstructionD10 currentInstruction = InstructionD10();
    int currentInstructionTimer = 0;

    // Process the instructions
    while (true) {
        ++cycle;

        // Start of cycle
        if (currentInstructionTimer == 0) {
            if (instructionIndex < instructions.size()) {
                currentInstruction = instructions[instructionIndex++];
                currentInstructionTimer = currentInstruction.opCode == OpCodes::Addx ? 2 : 1;
            }
            else
                break; // Reached end of instructions
        }

        // During the cycle
        sampler->sample(cycle, x);
        --currentInstructionTimer;

        // End of cycle
        if (currentInstructionTimer == 0) {
            if (currentInstruction.opCode == OpCodes::Addx)
                x += currentInstruction.operand;
        }
    }
}

Result Day10::runPart1() const
{
    SamplerPart1 sampler;
    runInstructions(instructions, &sampler);
    return sampler.getResult();
}

Result Day10::runPart2() const
{
    SamplerPart2 sampler;
    runInstructions(instructions, &sampler);
    return sampler.getResult();
}