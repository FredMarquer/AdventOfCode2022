#include "Rect.h"

#include <algorithm>

#include "Int2.h"
#include "Range.h"

const Rect Rect::Null(Int2(INT32_MAX), Int2(INT32_MIN));

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
