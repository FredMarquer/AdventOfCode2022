#include "Day09.h"

#include <algorithm>
#include <array>
#include <assert.h>
#include <charconv>
#include <cstdlib>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>

#include "Result.h"
#include "Int2.h"
#include "Log.h"

Int2 charToDirection(char c)
{
    switch (c) {
    case 'R': return Int2::Right;
    case 'L': return Int2::Left;
    case 'U': return Int2::Up;
    case 'D': return Int2::Down;
    default:
        error("invalid letter: {}", c);
        return Int2::Zero;
    }
}

bool tryParseMotion(const std::string_view& line, Motion& outMotion)
{
    // Parse the direction
    Int2 direction = charToDirection(line[0]);
    if (direction == Int2::Zero) {
        return false;
    }

    // Parse the distance
    std::string_view distanceView = line.substr(2);
    int distance = 0;
    std::from_chars(distanceView.data(), distanceView.data() + distanceView.size(), distance);
    if (distance <= 0) {
        error("invalid distance: {}", distanceView);
        return false;
    }

    outMotion = Motion(direction, distance);
    return true;
}

bool Day09::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        Motion motion;
        if (tryParseMotion(line, motion))
            motions.push_back(motion);
        else
            return false;
    }

    return true;
}

template<int N>
bool move(const Int2& direction, std::array<Int2, N>& rope)
{
    assert(direction.isUnit());
    
    // Move the head
    rope[0] += direction;

    // Propagate the move
    for (int i = 0; i < N - 1; ++i) {
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

template<int N>
int simulate(const std::vector<Motion>& motions)
{
    std::array<Int2, N> rope = { Int2::Zero };

    std::unordered_set<Int2> visitedPositions;
    visitedPositions.insert(rope.back());

    // Process the motions
    for (const Motion& motion : motions) {
        for (int i = 0; i < motion.distance; ++i) {
            if (move(motion.direction, rope)) {
                if (!visitedPositions.contains(rope.back()))
                    visitedPositions.insert(rope.back());
            }
        }
    }

    return visitedPositions.size();
}

Result Day09::runPart1() const
{
    return simulate<2>(motions);
}

Result Day09::runPart2() const
{
    return simulate<10>(motions);
}