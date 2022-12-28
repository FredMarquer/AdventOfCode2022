#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"
#include "Utils/Int2.h"

class Day15 : public Day
{
public:
	struct Report
	{
		Int2 sensor;
		Int2 beacon;

		Report(Int2 sensor, Int2 beacon);
	};

private:
	std::vector<Report> reports;

public:
	std::string getInputPath() const override { return "Inputs/15.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

