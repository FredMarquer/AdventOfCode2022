#pragma once

#include <fstream>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

#include "Result.h"
#include "Solver.h"

class Day21 : public Solver
{
public:
	enum class OpCodes
	{
		Add,
		Sub,
		Mul,
		Div,
		Equ,
	};

	struct Operation
	{
		OpCodes opCode;
		std::string left;
		std::string right;
	};

	struct Monkey
	{
		std::variant<int, Operation> variant;
		std::string parent;

		Monkey() : variant(0) {}
		Monkey(int number) : variant(number) {}
		Monkey(Operation operation) : variant(operation) {}
	};

private:
	std::unordered_map<std::string, int> nameToIndex;
	std::vector<Monkey> monkeys;

public:
	const char* getInputPath() const override { return "Inputs/21.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

