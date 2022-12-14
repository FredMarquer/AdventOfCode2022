#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"
#include "Utils/Int2.h"

struct Motion
{
	Int2 direction;
	int distance;

	Motion() : direction(Int2::Zero), distance(0) {}
	Motion(Int2 direction, int distance) : direction(direction), distance(distance) {}
};

class Day09 : public Day
{
private:
	std::vector<Motion> motions;

public:
	std::string getInputPath() const override { return "Inputs/09.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	bool tryGetExpectedResultPart1(Result& outResult) const override;
	bool tryGetExpectedResultPart2(Result& outResult) const override;
};

