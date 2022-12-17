#pragma once

#include <cstdint>

#include <assert.h>

struct Range
{
	int32_t min;
	int32_t max; // Exclusive

	static const Range Null;

	Range() : min(0), max(0) {}
	Range(int32_t min, int32_t max) : min(min), max(max) {}

	int32_t getSize() const { return max - min; }

	bool contains(int32_t value) const;
	bool contains(const Range& other) const;
	bool overlap(const Range& other) const;
	bool touch(const Range& other) const;
	void encapsulate(int32_t value);
	void encapsulate(const Range& other);
	void clamp(const Range& bounds);
};

