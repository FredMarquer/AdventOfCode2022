#pragma once

#include <fstream>
#include <string>

#include "Day.h"
#include "Result.h"

class Day06 : public Day
{
private:
	std::string signal;

public:
	std::string getInputPath() const override { return "inputs/06.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	bool tryGetExpectedResultPart1(Result& outResult) const override;
	bool tryGetExpectedResultPart2(Result& outResult) const override;
};

