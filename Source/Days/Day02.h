#pragma once

#include <array>
#include <fstream>
#include <utility>
#include <vector>

#include "Result.h"
#include "Solver.h"

class Day02 : public Solver
{
private:
	std::vector<std::pair<int, int>> strategyGuide;

public:
	const char* getInputPath() const override { return "Inputs/02.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

