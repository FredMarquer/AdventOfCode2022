#pragma once

#include <array>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "Day.h"
#include "Result.h"

class Day02 : public Day
{
private:
	std::vector<std::pair<int, int>> strategyGuide;

public:
	std::string getInputPath() const override { return "Inputs/02.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	bool tryGetExpectedResultPart1(Result& outResult) const override;
	bool tryGetExpectedResultPart2(Result& outResult) const override;
};

