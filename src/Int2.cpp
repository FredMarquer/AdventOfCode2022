#include "Int2.h"

#include <cstdlib>

const Int2 Int2::Zero(0, 0);
const Int2 Int2::Right(1, 0);
const Int2 Int2::Left(-1, 0);
const Int2 Int2::Up(0, 1);
const Int2 Int2::Down(0, -1);

bool Int2::isUnit() const
{
    return (y == 0 && abs(x) == 1) || (x == 0 && abs(y) == 1);
}

bool Int2::operator==(const Int2& rhs) const
{
    return x == rhs.x && y == rhs.y;
}

bool Int2::operator!=(const Int2& rhs) const
{
    return !(*this == rhs);
}

Int2 Int2::operator-() const
{
    return Int2(-x, -y);
}

Int2 Int2::operator+(const Int2& rhs) const
{
    return Int2(x + rhs.x, y + rhs.y);
}

Int2 Int2::operator-(const Int2& rhs) const
{
    return Int2(x - rhs.x, y - rhs.y);
}

Int2 Int2::operator+=(const Int2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Int2 Int2::operator-=(const Int2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Int2& lhs)
{
    os << '(' << lhs.x << ',' << lhs.y << ')';
    return os;
}

size_t std::hash<Int2>::operator()(Int2 const& rhs) const noexcept
{
    return ((rhs.x << 5) + rhs.x) + rhs.y;
}