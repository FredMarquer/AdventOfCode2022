#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

class Day05 : public Day
{
public:
	struct Instruction
	{
		int numberOfBoxes;
		int fromStackIndex;
		int toStackIndex;

		Instruction(int numberOfBoxes, int fromStackIndex, int toStackIndex)
			: numberOfBoxes(numberOfBoxes)
			, fromStackIndex(fromStackIndex)
			, toStackIndex(toStackIndex)
		{}
	};

private:
	std::vector<std::vector<char>> stacks;
	std::vector<Instruction> instructions;

public:
	std::string getInputPath() const override { return "Inputs/05.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

