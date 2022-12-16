#pragma once

#include <fstream>
#include <string>

#include "Day.h"
#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/Int2.h"

class Day08 : public Day
{
private:
	Array2D<int> treeMap;

public:
	std::string getInputPath() const override { return "Inputs/08.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;

private:
	void updateTreesVisibility(const Int2& start, const Int2& dir, Array2D<bool>& visibilityMap, int& visibilityCount) const;
	int computeScenicScore(const Int2& coord) const;
	int computeViewDistance(const Int2& start, const Int2& dir) const;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

