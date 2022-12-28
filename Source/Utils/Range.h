#pragma once

#include <algorithm>
#include <assert.h>
#include <cstdint>
#include <format>

struct Range
{
	int32_t min;
	int32_t max; // Exclusive

	Range() : min(0), max(0) {}
    Range(int32_t value) : min(value), max(value + 1) {}
    Range(int32_t min, int32_t max) : min(min), max(max) { assert(max >= min); }

	inline int32_t getSize() const { return max - min; }

    inline bool contains(int32_t value) const { return value >= min && value < max; }
    inline bool contains(Range other) const { return other.min >= min && other.max <= max; }
    inline bool overlap(Range other) const { return other.min < max && other.max > min; }
    inline bool touch(Range other) const { return other.min <= max && other.max >= min; }

    inline void encapsulate(int32_t value);
    inline void encapsulate(Range other);
    inline void clamp(Range bounds);
};

void Range::encapsulate(int32_t value)
{
    if (value < min)
        min = value;
    else if (value >= max)
        max = value + 1;
}

void Range::encapsulate(Range other)
{
    if (other.min < min)
        min = other.min;
    if (other.max > max)
        max = other.max;
}

void Range::clamp(Range bounds)
{
    min = std::clamp(min, bounds.min, bounds.max - 1);
    max = std::clamp(max, bounds.min, bounds.max);
}

template <>
struct std::formatter<Range>
{
    template <typename FormatParseContext>
    auto parse(FormatParseContext& pc)
    {
        return pc.end();
    }

    template<typename FormatContext>
    auto format(Range range, FormatContext& fc)
    {
        return std::format_to(fc.out(), "[{},{}[", range.min, range.max);
    }
};

