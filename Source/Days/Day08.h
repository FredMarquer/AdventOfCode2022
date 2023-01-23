#pragma once

#include <fstream>

#include "Result.h"
#include "Solver.h"
#include "Utils/Array2D.h"
#include "Utils/Int2.h"

class Day08 : public Solver
{
private:
	Array2D<int> treeMap;

public:
	const char* getInputPath() const override { return "Inputs/08.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;

private:
	void updateTreesVisibility(Int2 start, Int2 dir, Array2D<bool>& visibilityMap, int& visibilityCount) const;
	int computeScenicScore(Int2 coord) const;
	int computeViewDistance(Int2 start, Int2 dir) const;
};

