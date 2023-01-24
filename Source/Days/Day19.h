#pragma once

#include <fstream>
#include <vector>

#include "Result.h"
#include "Solver.h"

class Day19 : public Solver
{
public:
	struct Cost
	{
		std::array<int32_t, 4> costPerResources;

		Cost() : costPerResources{ 0 } {}
	};

	struct Blueprint {
		int32_t id;
		std::array<Cost, 4> robotCosts;
		std::array<int32_t, 4> maxRobotPerResources;

		Blueprint(int32_t id) : id(id), robotCosts{ Cost() }, maxRobotPerResources{ 0 } {}
	};

private:
	std::vector<Blueprint> blueprints;

public:
	const char* getInputPath() const override { return "Inputs/19.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

