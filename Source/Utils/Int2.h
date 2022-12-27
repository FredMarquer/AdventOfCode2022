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

	inline bool operator==(const Int2& rhs) const { return x == rhs.x && y == rhs.y; }
	inline bool operator!=(const Int2& rhs) const { return !(*this == rhs); }
	inline Int2 operator-() const { return Int2(-x, -y); }
	inline Int2 operator+(const Int2& rhs) const { return Int2(x + rhs.x, y + rhs.y); }
	inline Int2 operator-(const Int2& rhs) const { return Int2(x - rhs.x, y - rhs.y); }
	inline Int2 operator+=(const Int2& rhs);
	inline Int2 operator-=(const Int2& rhs);
};

bool Int2::isUnit() const
{
	return (y == 0 && abs(x) == 1) || (x == 0 && abs(y) == 1);
}

Int2 Int2::operator+=(const Int2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Int2 Int2::operator-=(const Int2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

template<>
struct std::hash<Int2>
{
	inline size_t operator()(const Int2& rhs) const noexcept
	{
		size_t hash = std::hash<int32_t>()(rhs.x);
		hash = ((hash << 5) + hash) + std::hash<int32_t>()(rhs.y);
		return hash;
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
	auto format(const Int2& value, FormatContext& fc)
	{
		return std::format_to(fc.out(), "({},{})", value.x, value.y);
	}
};

