#pragma once

#include <format>
#include <iostream>

struct Int2
{
	int32_t x, y;

	static const Int2 Zero;
	static const Int2 Right;
	static const Int2 Left;
	static const Int2 Up;
	static const Int2 Down;

	Int2() : x(0), y(0) {}
	Int2(int32_t value) : x(value), y(value) {}
	Int2(int32_t x, int32_t y) : x(x), y(y) {}

	bool isUnit() const;

	bool operator==(const Int2& rhs) const;
	bool operator!=(const Int2& rhs) const;
	Int2 operator-() const;
	Int2 operator+(const Int2& rhs) const;
	Int2 operator-(const Int2& rhs) const;
	Int2 operator+=(const Int2& rhs);
	Int2 operator-=(const Int2& rhs);
};

std::ostream& operator<<(std::ostream& os, const Int2& lhs);

template<>
struct std::hash<Int2>
{
	size_t operator()(const Int2& rhs) const noexcept;
};

template <>
struct std::formatter<Int2>
{
	template <typename FormatParseContext>
	auto parse(FormatParseContext& pc)
	{
		return pc.end();
	}

	template<typename FormatContext>
	auto format(const Int2& rhs, FormatContext& fc)
	{
		return std::format_to(fc.out(), "({}, {})", rhs.x, rhs.y);
	}
};
