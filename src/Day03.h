#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

struct Rucksack
{
public:
	std::string firstCompartment;
	std::string secondCompartment;

	Rucksack() {}
	Rucksack(Rucksack&& other) noexcept
		: firstCompartment(std::move(other.firstCompartment))
		, secondCompartment(std::move(other.secondCompartment))
	{}
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

