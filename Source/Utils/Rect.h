#pragma once

#include <algorithm>
#include <assert.h>
#include <cstdint>
#include <format>

#include "Int2.h"
#include "Range.h"

struct Rect
{
	Int2 min;
	Int2 max; // Exclusive

	Rect() {}
    Rect(const Int2& point) : min(point), max(point + 1) {}
	Rect(const Int2& min, const Int2& max) : min(min), max(max) { assert(max.x >= min.x); assert(max.y >= min.y); }

	inline int32_t getWidth() const { return max.x - min.x; }
	inline int32_t getHeight() const { return max.y - min.y; }
    inline int32_t getArea() const { return getWidth() * getHeight(); }
	inline Range getRangeX() const { return Range(min.x, max.x); }
	inline Range getRangeY() const { return Range(min.y, max.y); }

    inline bool contains(const Int2& point) const;
    inline bool contains(const Rect& other) const;
    inline bool overlap(const Rect& other) const;
    inline bool touch(const Rect& other) const;

    inline void encapsulate(const Int2& point);
    inline void encapsulate(const Rect& other);
    inline void encapsulateX(int32_t value);
    inline void encapsulateX(const Range& range);
    inline void encapsulateY(int32_t value);
    inline void encapsulateY(const Range& range);
    inline void clamp(const Rect& bounds);
    inline void clampX(const Range& bounds);
    inline void clampY(const Range& bounds);
};

bool Rect::contains(const Int2& point) const
{
    return
        point.x >= min.x &&
        point.x < max.x &&
        point.y >= min.y &&
        point.y < max.y;
}

bool Rect::contains(const Rect& other) const
{
    return
        other.min.x >= min.x &&
        other.max.x <= max.x &&
        other.min.y >= min.y &&
        other.max.y <= max.y;
}

bool Rect::overlap(const Rect& other) const
{
    return
        other.min.x < max.x &&
        other.max.x > min.x &&
        other.min.y < max.y &&
        other.max.y > min.y;
}

bool Rect::touch(const Rect& other) const
{
    return
        other.min.x <= max.x &&
        other.max.x >= min.x &&
        other.min.y <= max.y &&
        other.max.y >= min.y;
}

void Rect::encapsulate(const Int2& point)
{
    if (point.x < min.x)
        min.x = point.x;
    else if (point.x >= max.x)
        max.x = point.x + 1;

    if (point.y < min.y)
        min.y = point.y;
    else if (point.y >= max.y)
        max.y = point.y + 1;
}

void Rect::encapsulate(const Rect& other)
{
    encapsulateX(other.getRangeX());
    encapsulateY(other.getRangeY());
}

void Rect::encapsulateX(int32_t value)
{
    if (value < min.x)
        min.x = value;
    else if (value >= max.x)
        max.x = value + 1;
}

void Rect::encapsulateX(const Range& range)
{
    if (range.min < min.x)
        min.x = range.min;
    if (range.max > max.x)
        max.x = range.max;
}

void Rect::encapsulateY(int32_t value)
{
    if (value < min.y)
        min.y = value;
    else if (value >= max.y)
        max.y = value + 1;
}

void Rect::encapsulateY(const Range& range)
{
    if (range.min < min.y)
        min.y = range.min;
    if (range.max > max.y)
        max.y = range.max;
}

void Rect::clamp(const Rect& bounds)
{
    clampX(bounds.getRangeX());
    clampY(bounds.getRangeY());
}

void Rect::clampX(const Range& bounds)
{
    min.x = std::clamp(min.x, bounds.min, bounds.max - 1);
    max.x = std::clamp(max.x, bounds.min, bounds.max);
}

void Rect::clampY(const Range& bounds)
{
    min.y = std::clamp(min.y, bounds.min, bounds.max - 1);
    max.y = std::clamp(max.y, bounds.min, bounds.max);
}

template <>
struct std::formatter<Rect>
{
    template <typename FormatParseContext>
    auto parse(FormatParseContext& pc)
    {
        return pc.end();
    }

    template<typename FormatContext>
    auto format(const Rect& rect, FormatContext& fc)
    {
        return std::format_to(fc.out(), "[{},{}[", rect.min, rect.max);
    }
};

