#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Result.h"
#include "Solver.h"

class Day10 : public Solver
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

		Instruction();
		Instruction(OpCodes opCode);
		Instruction(OpCodes opCode, int operand);
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

