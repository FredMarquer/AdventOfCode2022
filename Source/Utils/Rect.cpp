#include "Rect.h"

#include "Int2.h"

const Rect Rect::Null(Int2(INT32_MAX), Int2(INT32_MIN));

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
        other.min.x <= max.x &&
        other.max.x >= min.x &&
        other.min.y <= max.y &&
        other.max.y >= min.y;
}

void Rect::encapsulate(const Int2& point)
{
    if (point.x < min.x)
        min.x = point.x;
    else if (point.x > max.x)
        max.x = point.x;

    if (point.y < min.y)
        min.y = point.y;
    else if (point.y > max.y)
        max.y = point.y;
}