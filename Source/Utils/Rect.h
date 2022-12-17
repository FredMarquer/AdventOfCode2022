#pragma once

#include <assert.h>

#include "Int2.h"
#include "Range.h"

struct Rect
{
	Int2 min;
	Int2 max; // Exclusive

	static const Rect Null;

	Rect() {}
	Rect(const Int2& min, const Int2& max) : min(min), max(max) {}

	int32_t getWidth() const { return max.x - min.x; }
	int32_t getHeight() const { return max.y - min.y; }
	Range getRangeX() const { return Range(min.x, max.x); }
	Range getRangeY() const { return Range(min.y, max.y); }

	bool contains(const Int2& other) const;
	bool contains(const Rect& other) const;
	bool overlap(const Rect& other) const;
	bool touch(const Rect& other) const;
	void encapsulate(const Int2& point);
	void encapsulate(const Rect& other);
	void encapsulateX(int32_t value);
	void encapsulateX(const Range& range);
	void encapsulateY(int32_t value);
	void encapsulateY(const Range& range);
	void clamp(const Rect& bounds);
	void clampX(const Range& bounds);
	void clampY(const Range& bounds);
};

