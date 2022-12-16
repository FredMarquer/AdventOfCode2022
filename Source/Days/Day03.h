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
		Rucksack(Rucksack&& other) noexcept
			: firstCompartment(std::move(other.firstCompartment))
			, secondCompartment(std::move(other.secondCompartment))
		{}
	};

private:
	std::vector<Rucksack> rucksacks;

public:
	std::string getInputPath() const override { return "Inputs/03.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	bool tryGetExpectedResultPart1(Result& outResult) const override;
	bool tryGetExpectedResultPart2(Result& outResult) const override;
};

