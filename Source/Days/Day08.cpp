#include "StdAfx.h"

#include "Day08.h"

#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/Exception.h"
#include "Utils/Int2.h"

void Day08::updateTreesVisibility(Int2 start, Int2 dir, Array2D<bool>& visibilityMap, int& visibilityCount) const
{
    assert(treeMap.isInBounds(start));
    assert(dir.isUnit());

    // Find all visible trees in this raw/column until we reach the border
    int visibilityHeight = -1;
    Int2 coord = start;
    do {
        // Is this tree higher than the previous highest visible tree ?
        size_t index = treeMap.getIndex(coord);
        int treeHeight = treeMap[index];
        if (treeHeight > visibilityHeight) {
            visibilityHeight = treeHeight;

            // Update the tree visibility
            if (!visibilityMap[index]) {
                visibilityMap[index] = true;
                ++visibilityCount;
            }
        }

        coord += dir;
    } while (treeMap.isInBounds(coord));
}

int Day08::computeScenicScore(Int2 coord) const
{
    assert(treeMap.isInBounds(coord));

    // Compute the distance in the 4 directions
    int distanceRight = computeViewDistance(coord, Int2::Right);
    int distanceLeft = computeViewDistance(coord, Int2::Left);
    int distanceUp = computeViewDistance(coord, Int2::Up);
    int distanceDown = computeViewDistance(coord, Int2::Down);

    // Compute and return the scenic score
    return distanceRight * distanceLeft * distanceUp * distanceDown;
}

int Day08::computeViewDistance(Int2 start, Int2 dir) const
{
    assert(treeMap.isInBounds(start));
    assert(dir.isUnit());

    // Compute the view distance
    int viewDistance = 0;
    int startHeight = treeMap[start];
    Int2 coord = start;
    while (true) {
        coord += dir;

        // Do we reach the border of the map ?
        if (!treeMap.isInBounds(coord))
            break;

        ++viewDistance;

        // Is this tree stopping the view ?
        if (treeMap[coord] >= startHeight)
            break;
    }

    return viewDistance;
}

void Day08::parseFile(std::ifstream& file)
{
    size_t width = 0;
    size_t height = 0;
    std::vector<int> treeData;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            exception("line is empty");

        // Update map width and height
        ++height;
        if (width == 0)
            width = line.size();

        if (line.size() != width)
            exception("line size (= {}) doesn't match the map width (= {})", line.size(), width);

        // Push the line into the tree map
        for (size_t i = 0; i < width; ++i) {
            char c = line[i];
            if (c < '0' || c > '9')
                exception("invalid char: {}", c);
            treeData.push_back(c - '0');
        }
    }

    treeMap = Array2D(width, height, treeData);
}

Result Day08::runPart1() const
{
    Array2D<bool> visibilityMap(treeMap.getWidth(), treeMap.getHeight());
    int visibilityCount = 0;

    int32_t width = (int32_t)treeMap.getWidth();
    int32_t height = (int32_t)treeMap.getHeight();

    for (int32_t x = 0; x < width; ++x) {
        updateTreesVisibility(Int2(x, 0), Int2::Up, visibilityMap, visibilityCount);
        updateTreesVisibility(Int2(x, height - 1), Int2::Down, visibilityMap, visibilityCount);
    }

    for (int32_t y = 0; y < height; ++y) {
        updateTreesVisibility(Int2(0, y), Int2::Right, visibilityMap, visibilityCount);
        updateTreesVisibility(Int2(width - 1, y), Int2::Left, visibilityMap, visibilityCount);
    }

    return visibilityCount;
}

Result Day08::runPart2() const
{
    // Find the best scenic score
    int bestScenicScore = 0;
    int32_t width = (int32_t)treeMap.getWidth();
    int32_t height = (int32_t)treeMap.getHeight();
    for (int32_t x = 0; x < width; ++x) {
        for (int32_t y = 0; y < height; ++y) {
            int scenicScore = computeScenicScore(Int2(x, y));
            if (scenicScore > bestScenicScore)
                bestScenicScore = scenicScore;
        }
    }

    return bestScenicScore;
}

Result Day08::getExpectedResultPart1() const
{
    return 1681;
}

Result Day08::getExpectedResultPart2() const
{
    return 201684;
}
