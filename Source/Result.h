#pragma once

#include <cstdint>
#include <format>
#include <string>
#include <variant>

struct Result
{
	friend std::formatter<Result>;

private:
	std::variant<std::monostate, int64_t, std::string> data;

public:
	static const Result Invalid;

	Result() : data(std::monostate()) {}
	Result(int32_t value) : data((int64_t)value) {}
	Result(uint32_t value) : data((int64_t)value) {}
	Result(int64_t value) : data(value) {}
	Result(uint64_t value) : data((int64_t)value) {}
	Result(const char* value) : data(std::string(value)) {}
	Result(std::string value) : data(value) {}

	inline bool operator==(const Result& rhs) const { return data == rhs.data; }
	inline bool operator!=(const Result& rhs) const { return !(*this == rhs); }

	inline bool isValid() const { return !std::holds_alternative<std::monostate>(this->data); }
	inline bool isInteger() const { return std::holds_alternative<int64_t>(this->data); }
	inline bool isString() const { return std::holds_alternative<std::string>(this->data); }
	inline int64_t getInteger() const { return std::get<int64_t>(this->data); }
	inline const std::string& getString() const { return std::get<std::string>(this->data); }
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
		if (result.isInteger())
			return std::format_to(fc.out(), "{}", result.getInteger());
		else if (result.isString())
			return std::format_to(fc.out(), "{}", result.getString());
		else
			return std::format_to(fc.out(), "Result::Invalid");
	}
};