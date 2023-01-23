#pragma once

#include <fstream>

#include "Result.h"

class Solver
{
public:
	virtual ~Solver() {}

	virtual const char* getInputPath() const = 0;
	virtual void parseFile(std::ifstream& file) = 0;
	virtual Result runPart1() const = 0;
	virtual Result runPart2() const = 0;
	virtual Result getExpectedResultPart1() const { return Result::Invalid; }
	virtual Result getExpectedResultPart2() const { return Result::Invalid; }
};

