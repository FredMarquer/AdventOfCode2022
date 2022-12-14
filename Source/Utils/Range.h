#pragma once

#include <cstdint>

#include <assert.h>

struct Range
{
	int32_t min;
	int32_t max;

	Range(int32_t min, int32_t max) : min(min), max(max) { assert(min <= max); }

	bool contains(const Range& other) const { return other.min >= min && other.max <= max; }
	bool overlap(const Range& other) const { return other.min <= max && other.max >= min; }
};