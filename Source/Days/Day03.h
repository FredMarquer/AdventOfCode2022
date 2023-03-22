#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Result.h"
#include "Solver.h"

class Day03 : public Solver
{
public:
	struct Rucksack
	{
		std::string firstCompartment;
		std::string secondCompartment;
	};

private:
	std::vector<Rucksack> rucksacks;

public:
	const char* getInputPath() const override { return "Inputs/03.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

