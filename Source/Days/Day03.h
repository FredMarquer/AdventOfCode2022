#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

class Day03 : public Day
{
public:
	struct Rucksack
	{
		std::string firstCompartment;
		std::string secondCompartment;

		Rucksack() {}
		Rucksack(Rucksack&& other) noexcept;
	};

private:
	std::vector<Rucksack> rucksacks;

public:
	std::string getInputPath() const override { return "Inputs/03.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

