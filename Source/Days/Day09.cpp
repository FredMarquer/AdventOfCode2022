#include "StdAfx.h"

#include "Day09.h"

#include "Result.h"
#include "Utils/Exception.h"
#include "Utils/Int2.h"
#include "Utils/Parsing.h"

namespace
{
    Int2 charToDirection(char c)
    {
        switch (c) {
        case 'R': return Int2::Right;
        case 'L': return Int2::Left;
        case 'U': return Int2::Up;
        case 'D': return Int2::Down;
        }

        exception("invalid letter: {}", c);
    }

    Day09::Motion parseMotion(std::string_view line)
    {
        // Parse the direction
        Int2 direction = charToDirection(line[0]);

        // Parse the distance
        int distance = 0;
        parse(line.substr(2), distance);

        return Day09::Motion(direction, distance);
    }

    template<size_t N>
    bool move(Int2 direction, std::array<Int2, N>& rope)
    {
        assert(direction.isUnit());

        // Move the head
        rope[0] += direction;

        // Propagate the move
        for (size_t i = 0; i < N - 1; ++i) {
            Int2 delta = rope[i] - rope[i + 1];
            if (abs(delta.x) <= 1 && abs(delta.y) <= 1)
                return false;

            // Move the next knot
            Int2 move;
            move.x = std::clamp(delta.x, -1, 1);
            move.y = std::clamp(delta.y, -1, 1);
            rope[i + 1] += move;
        }

        // Return true if the tail has moved
        return true;
    }

    template<size_t N>
    int32_t simulate(const std::vector<Day09::Motion>& motions)
    {
        std::array<Int2, N> rope = { Int2::Zero };

        std::unordered_set<Int2> visitedPositions(10000);
        visitedPositions.insert(rope.back());

        // Process the motions
        for (const Day09::Motion& motion : motions) {
            for (int i = 0; i < motion.distance; ++i) {
                if (move(motion.direction, rope)) {
                    if (!visitedPositions.contains(rope.back()))
                        visitedPositions.insert(rope.back());
                }
            }
        }

        return (int32_t)visitedPositions.size();
    }
}

Day09::Motion::Motion()
    : direction(Int2::Zero)
    , distance(0)
{}

Day09::Motion::Motion(Int2 direction, int distance)
    : direction(direction)
    , distance(distance)
{}

void Day09::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        Motion motion = parseMotion(line);
        motions.push_back(motion);
    }
}

Result Day09::runPart1() const
{
    return simulate<2>(motions);
}

Result Day09::runPart2() const
{
    return simulate<10>(motions);
}

Result Day09::getExpectedResultPart1() const
{
    return 6284;
}

Result Day09::getExpectedResultPart2() const
{
    return 2661;
}
