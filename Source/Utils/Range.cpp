#include "Range.h"

#include <algorithm>

const Range Range::Null(INT32_MAX, INT32_MIN);

bool Range::contains(int32_t value) const
{
    return value >= min && value < max;
}

bool Range::contains(const Range& other) const
{ 
    return other.min >= min && other.max <= max;
}

bool Range::overlap(const Range& other) const
{
    return other.min < max && other.max > min;
}

bool Range::touch(const Range& other) const
{
    return other.min <= max && other.max >= min;
}

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
