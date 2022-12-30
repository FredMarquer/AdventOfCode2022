#pragma once

#include <fstream>
#include <string>

#include "Result.h"
#include "Solver.h"
#include "Utils/Array2D.h"
#include "Utils/Int2.h"

class Day14 : public Solver
{
private:
	Array2D<bool> caveMap;
	Int2 caveOffset;

public:
	std::string getInputPath() const override { return "Inputs/14.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

