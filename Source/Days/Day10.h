#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

enum class OpCodes
{
	Noop,
	Addx,
};

struct InstructionD10
{
	OpCodes opCode;
	int operand;

	InstructionD10() : opCode(OpCodes::Noop), operand(0) {}
	InstructionD10(OpCodes opCode) : opCode(opCode), operand(0) {}
	InstructionD10(OpCodes opCode, int operand) : opCode(opCode), operand(operand) {}
};

class Day10 : public Day
{
private:
	std::vector<InstructionD10> instructions;

public:
	std::string getInputPath() const override { return "Inputs/10.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	bool tryGetExpectedResultPart1(Result& outResult) const override;
	bool tryGetExpectedResultPart2(Result& outResult) const override;
};

