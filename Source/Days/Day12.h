#pragma once

#include <fstream>

#include "Result.h"
#include "Solver.h"
#include "Utils/Array2D.h"
#include "Utils/Int2.h"

class Day12 : public Solver
{
private:
	Int2 start;
	Int2 target;
	Array2D<int> heightMap;

public:
	const char* getInputPath() const override { return "Inputs/12.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

