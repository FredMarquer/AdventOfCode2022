#pragma once

#include <fstream>
#include <string>

#include "Result.h"

class Day
{
public:
	virtual std::string getInputPath() const = 0;
	virtual bool parseFile(std::ifstream& file) = 0;
	virtual Result runPart1() const = 0;
	virtual Result runPart2() const = 0;
	virtual bool tryGetExpectedResultPart1(Result& outResult) const { return false; }
	virtual bool tryGetExpectedResultPart2(Result& outResult) const { return false; }
};

