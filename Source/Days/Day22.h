#pragma once

#include <fstream>
#include <vector>

#include "Result.h"
#include "Solver.h"
#include "Utils/Array2D.h"

class Day22 : public Solver
{
public:
	enum class Tiles
	{
		OutOfMap,
		Empty,
		Wall,
	};

	enum class MoveInstructionTypes
	{
		Move,
		RotateRight,
		RotateLeft,
	};

	struct MoveInstruction
	{
		MoveInstructionTypes type;
		int operand;
	};

private:
	Array2D<Tiles> map;
	std::vector<MoveInstruction> moveInstructions;

public:
	const char* getInputPath() const override { return "Inputs/22.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

