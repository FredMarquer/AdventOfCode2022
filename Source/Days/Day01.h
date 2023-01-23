#pragma once

#include <fstream>
#include <vector>

#include "Result.h"
#include "Solver.h"

class Day01 : public Solver
{
private:
	std::vector<int> caloriesPerElves;

public:
	const char* getInputPath() const override { return "Inputs/01.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

