#pragma once

#include <fstream>
#include <vector>

#include "Result.h"
#include "Solver.h"
#include "Utils/Int2.h"

class Day09 : public Solver
{
public:
	struct Motion
	{
		Int2 direction;
		int distance;

		Motion();
		Motion(Int2 direction, int distance);
	};

private:
	std::vector<Motion> motions;

public:
	const char* getInputPath() const override { return "Inputs/09.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

