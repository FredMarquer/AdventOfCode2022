#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

class Day13 : public Day
{
public:
	struct Node
	{
		bool isInteger;
		int integer;
		std::vector<Node> list;

		Node() : isInteger(false), integer(0) {}
		Node(int integer) : isInteger(true), integer(integer) {}
	};

	struct Packet
	{
		std::string line;
		Node rootNode;

		bool operator==(const std::string& value) const { return line == value; }
		bool operator<(const Packet& rhs) const;
	};

private:
	std::vector<Packet> packets;

public:
	std::string getInputPath() const override { return "Inputs/13.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	bool tryGetExpectedResultPart1(Result& outResult) const override;
	bool tryGetExpectedResultPart2(Result& outResult) const override;
};

