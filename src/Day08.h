#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Int2.h"

class Day08 : public Day
{
private:
	int width, height;
	std::vector<int> treeMap;

public:
	std::string getInputPath() const override { return "inputs/08.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;

private:
	size_t getIndex(const Int2& coord) const;
	bool isInMap(const Int2& coord) const;
	void updateTreesVisibility(const Int2& start, const Int2& dir, std::vector<bool>& visibilityMap, int& visibilityCount) const;
	int computeScenicScore(const Int2& coord) const;
	int computeViewDistance(const Int2& start, const Int2& dir) const;
};

