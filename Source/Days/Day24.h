#pragma once

#include <fstream>
#include <vector>

#include "Result.h"
#include "Solver.h"
#include "Utils/Array2D.h"
#include "Utils/Int2.h"

class Day24 : public Solver
{
private:
	std::vector<Array2D<bool>> graph;
	Int2 startingPosition;
	Int2 targetPosition;

public:
	const char* getInputPath() const override { return "Inputs/24.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

