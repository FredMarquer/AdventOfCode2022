#pragma once

#include <fstream>
#include <functional>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

class Day11 : public Day
{
public:
	typedef std::function<void(int64_t&)> ItemModifierFunction;

	struct Monkey
	{
		std::vector<int64_t> startingItems;
		ItemModifierFunction inspectFunction;
		int64_t testDivisor;
		size_t monkeyIndexIfTrue;
		size_t monkeyIndexIfFalse;

		Monkey();
	};

private:
	std::vector<Monkey> monkeys;

public:
	std::string getInputPath() const override { return "Inputs/11.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

