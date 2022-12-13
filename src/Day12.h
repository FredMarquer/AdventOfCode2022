#pragma once

#include <fstream>
#include <string>

#include "Array2D.h"
#include "Day.h"
#include "Int2.h"
#include "Result.h"

class Day12 : public Day
{
private:
	Int2 start;
	Int2 target;
	Array2D<int> heightMap;

public:
	std::string getInputPath() const override { return "inputs/12.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
};

