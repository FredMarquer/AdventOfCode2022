#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"
#include "Utils/Int2.h"

class Day09 : public Day
{
public:
	struct Motion
	{
		Int2 direction;
		int distance;

		Motion() : direction(Int2::Zero), distance(0) {}
		Motion(Int2 direction, int distance) : direction(direction), distance(distance) {}
	};

private:
	std::vector<Motion> motions;

public:
	std::string getInputPath() const override { return "Inputs/09.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

