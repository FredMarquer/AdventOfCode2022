#pragma once

#include <cstdint>
#include <format>

struct Int2
{
	int32_t x, y;

	static const Int2 Zero;
	static const Int2 One;
	static const Int2 Right;
	static const Int2 Left;
	static const Int2 Up;
	static const Int2 Down;

	Int2() : x(0), y(0) {}
	Int2(int32_t value) : x(value), y(value) {}
	Int2(int32_t x, int32_t y) : x(x), y(y) {}

	inline bool isUnit() const;

	inline bool operator==(Int2 other) const { return x == other.x && y == other.y; }
	inline bool operator!=(Int2 other) const { return !(*this == other); }
	inline Int2 operator-() const { return Int2(-x, -y); }
	inline Int2 operator+(Int2 other) const { return Int2(x + other.x, y + other.y); }
	inline Int2 operator-(Int2 other) const { return Int2(x - other.x, y - other.y); }
	inline Int2 operator+=(Int2 other);
	inline Int2 operator-=(Int2 other);
};

bool Int2::isUnit() const
{
	return (y == 0 && abs(x) == 1) || (x == 0 && abs(y) == 1);
}

Int2 Int2::operator+=(Int2 other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Int2 Int2::operator-=(Int2 other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

template<>
struct std::hash<Int2>
{
	inline size_t operator()(Int2 value) const noexcept
	{
		size_t hash1 = std::hash<int32_t>()(value.x);
		size_t hash2 = std::hash<int32_t>()(value.y);
		return hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
	}
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
	auto format(Int2 value, FormatContext& fc)
	{
		return std::format_to(fc.out(), "({},{})", value.x, value.y);
	}
};

