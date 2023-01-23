#pragma once

#include <fstream>
#include <vector>

#include "Result.h"
#include "Solver.h"
#include "Utils/Int3.h"

class Day18 : public Solver
{
private:
	std::vector<Int3> positions;
	int32_t maxX = 0;
	int32_t maxY = 0;
	int32_t maxZ = 0;

public:
	const char* getInputPath() const override { return "Inputs/18.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

