#include "Day08.h"

#include <assert.h>
#include <fstream>
#include <string>
#include <vector>

#include "Int2.h"
#include "Log.h"
#include "Result.h"

int charToInt(char c)
{
    if (c < '0' || c > '9') {
        error("invalid char: {}", c);
        return -1;
    }

    return c - '0';
}

bool Day08::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            error("line is empty");
            return false;
        }

        // Update map width and height
        ++height;
        if (width == 0)
            width = line.size();

        if (line.size() != width) {
            error("line size (= {}) doesn't match the map width (= {})", line.size(), width);
            return false;
        }

        // Push the line into the tree map
        for (int i = 0; i < width; ++i) {
            int treeHeight = charToInt(line[i]);
            if (treeHeight < 0)
                return false;

            treeMap.push_back(treeHeight);
        }
    }

    return true;
}

Result Day08::runPart1() const
{
    std::vector<bool> visibilityMap(treeMap.size());
    int visibilityCount = 0;

    for (int x = 0; x < width; ++x) {
        updateTreesVisibility(Int2(x, 0), Int2::Up, visibilityMap, visibilityCount);
        updateTreesVisibility(Int2(x, height - 1), Int2::Down, visibilityMap, visibilityCount);
    }

    for (int y = 0; y < height; ++y) {
        updateTreesVisibility(Int2(0, y), Int2::Right, visibilityMap, visibilityCount);
        updateTreesVisibility(Int2(width - 1, y), Int2::Left, visibilityMap, visibilityCount);
    }

    return visibilityCount;
}

Result Day08::runPart2() const
{
    // Find the best scenic score
    int bestScenicScore = 0;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int scenicScore = computeScenicScore(Int2(x, y));
            if (scenicScore > bestScenicScore)
                bestScenicScore = scenicScore;
        }
    }

    return bestScenicScore;
}

size_t Day08::getIndex(const Int2& coord) const
{
    assert(isInMap(coord));
    return coord.x + coord.y * width;
}

bool Day08::isInMap(const Int2& coord) const
{
    return 
        coord.x >= 0 && coord.x < width &&
        coord.y >= 0 && coord.y < height;
}

void Day08::updateTreesVisibility(const Int2& start, const Int2& dir, std::vector<bool>& visibilityMap, int& visibilityCount) const
{
    assert(isInMap(start));
    assert(dir.isUnit());

    // Find all visible trees in this raw/column until we reach the border
    int visibilityHeight = -1;
    Int2 coord = start;
    do {
        // Is this tree higher the previous highest visible tree ?
        int index = getIndex(coord);
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
    } while (isInMap(coord));
}

int Day08::computeScenicScore(const Int2& coord) const
{
    assert(isInMap(coord));

    // Compute the distance in the 4 directions
    int distanceRight = computeViewDistance(coord, Int2::Right);
    int distanceLeft = computeViewDistance(coord, Int2::Left);
    int distanceUp = computeViewDistance(coord, Int2::Up);
    int distanceDown = computeViewDistance(coord, Int2::Down);

    // Compute and return the scenic score
    return distanceRight * distanceLeft * distanceUp * distanceDown;
}

int Day08::computeViewDistance(const Int2& start, const Int2& dir) const
{
    assert(isInMap(start));
    assert(dir.isUnit());

    int startIndex = getIndex(start);
    int startHeight = treeMap[startIndex];

    // Compute the view distance
    int viewDistance = 0;
    Int2 coord = start;
    while (true) {
        coord += dir;

        // Do we reach the border of the map ?
        if (!isInMap(coord))
            break;

        ++viewDistance;

        // Is this tree stopping the view ?
        int index = getIndex(coord);
        int treeHeight = treeMap[index];
        if (treeHeight >= startHeight)
            break;
    }

    return viewDistance;
}
