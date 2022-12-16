#pragma once

#include <fstream>
#include <string>

#include "Day.h"
#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/Int2.h"

class Day12 : public Day
{
private:
	Int2 start;
	Int2 target;
	Array2D<int> heightMap;

public:
	std::string getInputPath() const override { return "Inputs/12.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	bool tryGetExpectedResultPart1(Result& outResult) const override;
	bool tryGetExpectedResultPart2(Result& outResult) const override;
};

