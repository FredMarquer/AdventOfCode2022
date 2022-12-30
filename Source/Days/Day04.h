#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "Result.h"
#include "Solver.h"
#include "Utils/Range.h"

class Day04 : public Solver
{
private:
	std::vector<std::pair<Range, Range>> pairs;

public:
	std::string getInputPath() const override { return "Inputs/04.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

