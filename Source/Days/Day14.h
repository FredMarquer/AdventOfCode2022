#pragma once

#include <fstream>
#include <string>

#include "Day.h"
#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/Int2.h"

class Day14 : public Day
{
private:
	Array2D<bool> caveMap;
	Int2 caveOffset;

public:
	std::string getInputPath() const override { return "Inputs/14.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	bool tryGetExpectedResultPart1(Result& outResult) const override;
	bool tryGetExpectedResultPart2(Result& outResult) const override;
};

