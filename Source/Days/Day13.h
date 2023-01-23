#pragma once

#include <fstream>
#include <span>
#include <string>
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

		operator std::span<const Node>() const;

		bool isInteger() const;
		bool isList() const;
		int getInteger() const;
		std::vector<Node>& getList();
		const std::vector<Node>& getList() const;
	};

	struct Packet
	{
		std::string line;
		Node rootNode;

		bool operator==(const std::string& value) const;
		bool operator<(const Packet& rhs) const;
	};

private:
	std::vector<Packet> packets;

public:
	const char* getInputPath() const override { return "Inputs/13.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

