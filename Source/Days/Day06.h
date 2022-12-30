#pragma once

#include <fstream>
#include <string>

#include "Result.h"
#include "Solver.h"

class Day06 : public Solver
{
private:
	std::string signal;

public:
	std::string getInputPath() const override { return "Inputs/06.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

