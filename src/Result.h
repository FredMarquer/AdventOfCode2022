#pragma once

#include <string>

struct Result
{
	friend std::ostream& operator<<(std::ostream& stream, const Result& result);

private:
	int64_t intValue;
	std::string stringValue;

public:
	Result(int64_t intValue) : intValue(intValue) {}
	Result(std::string stringValue) : intValue(0), stringValue(stringValue) {}
};

