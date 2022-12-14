#pragma once

#include <assert.h>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "Day.h"
#include "Result.h"
#include "Utils/Range.h"

class Day04 : public Day
{
private:
	std::vector<std::pair<Range, Range>> pairs;

public:
	std::string getInputPath() const override { return "Inputs/04.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	bool tryGetExpectedResultPart1(Result& outResult) const override;
	bool tryGetExpectedResultPart2(Result& outResult) const override;
};

