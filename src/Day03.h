#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

struct Rucksack {
	std::string firstCompartment;
	std::string secondCompartment;
};

class Day03 : public Day
{
private:
	std::vector<Rucksack> rucksacks;

public:
	std::string getInputPath() const override { return "inputs/03.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
};

