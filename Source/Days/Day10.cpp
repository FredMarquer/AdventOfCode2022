#include "Day10.h"

#include <assert.h>
#include <fstream>
#include <string>
#include <vector>

#include "Result.h"
#include "Utils/Exception.h"
#include "Utils/Parsing.h"

Day10::Instruction parseInstruction(const std::string_view& line)
{
    // Parse the op code first
    std::string_view opCodeView = line.substr(0, 4);
    if (opCodeView == "noop") {
        return Day10::Instruction(Day10::OpCodes::Noop);
    }
    else if (opCodeView == "addx") {
        // Parse the operand
        int operand = 0;
        parse(line.substr(5), operand);
        return Day10::Instruction(Day10::OpCodes::Addx, operand);
    }
    
    exception("invalid line: {}", line);
}

void Day10::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        Day10::Instruction instruction = parseInstruction(line);
        instructions.push_back(instruction);
    }
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

void runInstructions(const std::vector<Day10::Instruction>& instructions, Sampler* sampler)
{
    assert(sampler != nullptr);

    int cycle = 0;
    int x = 1;

    size_t instructionIndex = 0;
    Day10::Instruction currentInstruction = Day10::Instruction();
    int currentInstructionTimer = 0;

    // Process the instructions
    while (true) {
        ++cycle;

        // Start of cycle
        if (currentInstructionTimer == 0) {
            if (instructionIndex < instructions.size()) {
                currentInstruction = instructions[instructionIndex++];
                currentInstructionTimer = currentInstruction.opCode == Day10::OpCodes::Addx ? 2 : 1;
            }
            else
                break; // Reached end of instructions
        }

        // During the cycle
        sampler->sample(cycle, x);
        --currentInstructionTimer;

        // End of cycle
        if (currentInstructionTimer == 0) {
            if (currentInstruction.opCode == Day10::OpCodes::Addx)
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

bool Day10::tryGetExpectedResultPart1(Result& outResult) const
{
    outResult = 11820;
    return true;
}

bool Day10::tryGetExpectedResultPart2(Result& outResult) const
{
    outResult =
        "\n####.###....##.###..###..#..#..##..#..#."
        "\n#....#..#....#.#..#.#..#.#.#..#..#.#..#."
        "\n###..#..#....#.###..#..#.##...#..#.####."
        "\n#....###.....#.#..#.###..#.#..####.#..#."
        "\n#....#....#..#.#..#.#.#..#.#..#..#.#..#."
        "\n####.#.....##..###..#..#.#..#.#..#.#..#.";
    return true;
}
