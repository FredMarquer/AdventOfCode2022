#pragma once

#include <format>
#include <string>

struct Result
{
	friend std::ostream& operator<<(std::ostream& stream, const Result& result);
	friend std::formatter<Result>;

private:
	int64_t intValue;
	std::string stringValue;

public:
	Result() : intValue(0) {}
	Result(int64_t value) : intValue(value) {}
	Result(const char* value) : intValue(0), stringValue(value) {}
	Result(std::string value) : intValue(0), stringValue(value) {}

	bool operator==(const Result& rhs) const;
	bool operator!=(const Result& rhs) const;
};

template <>
struct std::formatter<Result>
{
	template <typename FormatParseContext>
	auto parse(FormatParseContext& pc)
	{
		return pc.end();
	}

	template<typename FormatContext>
	auto format(const Result& result, FormatContext& fc)
	{
		if (result.stringValue.empty()) {
			return std::format_to(fc.out(), "{}", result.intValue);
		}
		else {
			return std::format_to(fc.out(), "{}", result.stringValue);
		}
	}
};