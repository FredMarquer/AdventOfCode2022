#pragma once

#include <array>
#include <fstream>
#include <vector>

#include "Result.h"
#include "Solver.h"
#include "Utils/Int2.h"

class Day17 : public Solver
{
public:
	struct Rock
	{
		std::array<Int2, 5> blocks;
		size_t size;
		int32_t height;

		Rock(std::initializer_list<Int2> b);

		std::array<Int2, 5>::const_iterator begin() const;
		std::array<Int2, 5>::const_iterator end() const;
	};

private:
	std::vector<Rock> rocks;
	std::vector<Int2> jets;

public:
	const char* getInputPath() const override { return "Inputs/17.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

