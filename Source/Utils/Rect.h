#pragma once

#include <assert.h>

#include "Int2.h"

struct Rect
{
	Int2 min;
	Int2 max;

	static const Rect Null;

	Rect() {}
	Rect(const Int2& min, const Int2& max) : min(min), max(max) {}

	int32_t getWidth() const { return max.x - min.x; }
	int32_t getHeight() const { return max.y - min.y; }

	bool contains(const Rect& other) const;
	bool overlap(const Rect& other) const;
	void encapsulate(const Int2& point);
};