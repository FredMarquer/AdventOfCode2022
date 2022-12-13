#pragma once

#include <fstream>
#include <string>

#include "Array2D.h"
#include "Day.h"
#include "Int2.h"
#include "Result.h"

class Day08 : public Day
{
private:
	Array2D<int> treeMap;

public:
	std::string getInputPath() const override { return "inputs/08.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;

private:
	void updateTreesVisibility(const Int2& start, const Int2& dir, Array2D<bool>& visibilityMap, int& visibilityCount) const;
	int computeScenicScore(const Int2& coord) const;
	int computeViewDistance(const Int2& start, const Int2& dir) const;
};

