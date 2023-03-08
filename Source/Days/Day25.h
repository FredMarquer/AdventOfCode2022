#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Result.h"
#include "Solver.h"

class Day25 : public Solver
{
private:
	std::vector<std::string> numbers;

public:
	const char* getInputPath() const override { return "Inputs/25.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

