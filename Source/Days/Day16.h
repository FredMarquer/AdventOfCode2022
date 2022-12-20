#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

class Day16 : public Day
{
public:
	struct ConnectedValve
	{
		std::string name;
		size_t index;
		int32_t cost;

		ConnectedValve(std::string name) : name(name), index(0), cost(1) {}
		ConnectedValve(std::string name, size_t index, int32_t cost) : name(name), index(index), cost(cost) {}
	};

	struct Valve
	{
		std::string name;
		size_t index;
		uint64_t bit;
		int32_t flowRate;
		std::vector<ConnectedValve> connectedValves;

		Valve(std::string&& name, size_t index, int flowRate) : name(std::move(name)), index(index), bit((uint64_t)1 << index), flowRate(flowRate) {}
	};

private:
	size_t startingValveIndex;
	std::vector<Valve> valves;
	int32_t totalFlowRate;

public:
	std::string getInputPath() const override { return "Inputs/16.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

