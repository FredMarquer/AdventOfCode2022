#pragma once

#include <fstream>
#include <span>
#include <variant>
#include <vector>

#include "Result.h"
#include "Solver.h"

class Day13 : public Solver
{
public:
	struct Node
	{
		std::variant<int, std::vector<Node>> data;

		Node();
		Node(int integer);

		bool isInteger() const;
		bool isList() const;
		int getInteger() const;
		std::vector<Node>& getList();
		const std::vector<Node>& getList() const;

		std::span<const Node> toSpan() const;

		bool operator<(const Node& rhs) const;
	};

private:
	std::vector<Node> packets;

public:
	const char* getInputPath() const override { return "Inputs/13.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

