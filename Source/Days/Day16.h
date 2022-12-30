#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Result.h"
#include "Solver.h"

class Day16 : public Solver
{
public:
	struct ConnectedValve
	{
		std::string name;
		size_t index;

		ConnectedValve(std::string&& name);
	};

	struct Valve
	{
		std::string name;
		size_t index;
		uint64_t bit;
		int32_t flowRate;
		std::vector<ConnectedValve> connectedValves;

		Valve(std::string&& name, int flowRate);

		bool operator<(const Valve& other) const;
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

