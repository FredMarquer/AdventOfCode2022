#pragma once

#include <fstream>
#include <vector>

#include "Result.h"
#include "Solver.h"

class Day05 : public Solver
{
public:
	struct Instruction
	{
		int numberOfBoxes;
		int fromStackIndex;
		int toStackIndex;

		Instruction(int numberOfBoxes, int fromStackIndex, int toStackIndex);
	};

private:
	std::vector<std::vector<char>> stacks;
	std::vector<Instruction> instructions;

public:
	const char* getInputPath() const override { return "Inputs/05.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

