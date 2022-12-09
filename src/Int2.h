#pragma once

#include <functional>
#include <iostream>

struct Int2
{
	int x, y;

	static const Int2 Zero;
	static const Int2 Right;
	static const Int2 Left;
	static const Int2 Up;
	static const Int2 Down;

	Int2() : x(0), y(0) {}
	Int2(int x, int y) : x(x), y(y) {}

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
	size_t operator()(Int2 const& s) const noexcept;
};