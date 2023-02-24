#pragma once

#include <fstream>
#include <vector>

#include "Result.h"
#include "Solver.h"
#include "Utils/Int2.h"

class Day23 : public Solver
{
private:
	std::vector<Int2> elvesPositions;
	int32_t startingWidth;
	int32_t startingHeight;

public:
	const char* getInputPath() const override { return "Inputs/23.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

