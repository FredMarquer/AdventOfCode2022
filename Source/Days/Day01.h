#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

class Day01 : public Day
{
private:
	std::vector<int> caloriesPerElves;

public:
	std::string getInputPath() const override { return "Inputs/01.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

