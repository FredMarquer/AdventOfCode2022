#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

class Day10 : public Day
{
public:
	enum class OpCodes
	{
		Noop,
		Addx,
	};

	struct Instruction
	{
		OpCodes opCode;
		int operand;

		Instruction() : opCode(OpCodes::Noop), operand(0) {}
		Instruction(OpCodes opCode) : opCode(opCode), operand(0) {}
		Instruction(OpCodes opCode, int operand) : opCode(opCode), operand(operand) {}
	};


private:
	std::vector<Instruction> instructions;

public:
	std::string getInputPath() const override { return "Inputs/10.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

