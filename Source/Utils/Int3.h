#pragma once

#include <cstdint>
#include <format>

struct Int3
{
	int32_t x, y, z;

	static const Int3 Zero;
	static const Int3 One;
	static const Int3 Right;
	static const Int3 Left;
	static const Int3 Up;
	static const Int3 Down;
	static const Int3 Forward;
	static const Int3 Backward;

	Int3() : x(0), y(0), z(0) {}
	Int3(int32_t value) : x(value), y(value), z(value) {}
	Int3(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z) {}

	inline bool isUnit() const;

	inline bool operator==(Int3 other) const { return x == other.x && y == other.y && z == other.z; }
	inline bool operator!=(Int3 other) const { return !(*this == other); }
	inline Int3 operator-() const { return Int3(-x, -y, -z); }
	inline Int3 operator+(Int3 other) const { return Int3(x + other.x, y + other.y, z + other.z); }
	inline Int3 operator-(Int3 other) const { return Int3(x - other.x, y - other.y, z - other.z); }
	inline Int3 operator+=(Int3 other);
	inline Int3 operator-=(Int3 other);
};

bool Int3::isUnit() const
{
	return (abs(x) + abs(y) + abs(z)) == 1;
}

Int3 Int3::operator+=(Int3 other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Int3 Int3::operator-=(Int3 other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

template<>
struct std::hash<Int3>
{
	inline size_t operator()(Int3 value) const noexcept
	{
		size_t hash = std::hash<int32_t>()(value.x);
		hash = std::hash<int32_t>()(value.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		hash = std::hash<int32_t>()(value.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		return hash;
	}
};

template <>
struct std::formatter<Int3>
{
	template <typename FormatParseContext>
	auto parse(FormatParseContext& pc)
	{
		return pc.end();
	}

	template<typename FormatContext>
	auto format(Int3 value, FormatContext& fc)
	{
		return std::format_to(fc.out(), "({},{},{})", value.x, value.y, value.z);
	}
};

