#pragma once

#include <algorithm>
#include <cstdint>
#include <format>

struct Range
{
	int32_t min;
	int32_t max; // Exclusive

	static const Range Null;

	Range() : min(0), max(0) {}
	Range(int32_t min, int32_t max) : min(min), max(max) {}

	inline int32_t getSize() const { return max - min; }

    inline bool contains(int32_t value) const { return value >= min && value < max; }
    inline bool contains(const Range& other) const { return other.min >= min && other.max <= max; }
    inline bool overlap(const Range& other) const { return other.min < max&& other.max > min; }
    inline bool touch(const Range& other) const { return other.min <= max && other.max >= min; }

    inline void encapsulate(int32_t value);
    inline void encapsulate(const Range& other);
    inline void clamp(const Range& bounds);
};

void Range::encapsulate(int32_t value)
{
    if (value < min)
        min = value;
    else if (value >= max)
        max = value + 1;
}

void Range::encapsulate(const Range& other)
{
    if (other.min < min)
        min = other.min;
    if (other.max > max)
        max = other.max;
}

void Range::clamp(const Range& bounds)
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
    auto format(const Range& range, FormatContext& fc)
    {
        return std::format_to(fc.out(), "[{},{}[", range.min, range.max);
    }
};

