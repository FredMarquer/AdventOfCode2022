#pragma once

#include <fstream>
#include <string>
#include <variant>
#include <vector>

#include "Day.h"
#include "Result.h"

class Day13 : public Day
{
public:
	struct Node
	{
		std::variant<int, std::vector<Node>> data;

		Node() : data(std::vector<Node>()) {}
		Node(int integer) : data(integer) {}

		bool isInteger() const { return std::holds_alternative<int>(this->data); }
		bool isList() const { return std::holds_alternative<std::vector<Day13::Node>>(this->data); }
		int getInteger() const { return std::get<int>(this->data); }
		std::vector<Node>& getList() { return std::get<std::vector<Day13::Node>>(this->data); }
		const std::vector<Node>& getList() const { return std::get<std::vector<Day13::Node>>(this->data); }
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
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	bool tryGetExpectedResultPart1(Result& outResult) const override;
	bool tryGetExpectedResultPart2(Result& outResult) const override;
};

