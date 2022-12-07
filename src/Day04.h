#pragma once

#include <assert.h>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "Day.h"
#include "Result.h"

struct Range
{
public:
	int min;
	int max;

	Range(int min, int max) : min(min), max(max) { assert(min <= max); }

	bool contains(const Range& other) const { return other.min >= min && other.max <= max; }
	bool overlap(const Range& other) const { return other.min <= max && other.max >= min; }
};

class Day04 : public Day
{
private:
	std::vector<std::pair<Range, Range>> pairs;

public:
	std::string getInputPath() const override { return "inputs/04.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
};

