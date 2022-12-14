#include "Day12.h"

#include <assert.h>
#include <fstream>
#include <functional>
#include <string>
#include <vector>

#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/BinaryHeap.h"
#include "Utils/Int2.h"
#include "Utils/Log.h"

bool tryParseChar(char c, int& height, bool& isStart, bool& isTarget)
{
    isStart = false;
    isTarget = false;

    if (c == 'S') {
        isStart = true;
        c = 'a';
    }
    else if (c == 'E') {
        isTarget = true;
        c = 'z';
    }
    else if (c < 'a' || c > 'z') {
        error("invalid char: {}", c);
        return false;
    }

    height = c - 'a';
    return true;
}

bool Day12::parseFile(std::ifstream& file)
{
    size_t mapWidth = 0;
    size_t mapHeight = 0;
    std::vector<int> heightData;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            error("line is empty");
            return false;
        }

        // Push the line into the height map
        for (size_t i = 0; i < line.size(); ++i) {
            int cellHeight;
            bool isStart, isTarget;
            if (!tryParseChar(line[i], cellHeight, isStart, isTarget))
                return false;
            
            if (isStart)
                start = Int2(i, mapHeight);

            if (isTarget)
                target = Int2(i, mapHeight);

            heightData.push_back(cellHeight);
        }

        // Update map width and height
        ++mapHeight;
        if (mapWidth == 0)
            mapWidth = line.size();

        if (line.size() != mapWidth) {
            error("line size (= {}) doesn't match the map width (= {})", line.size(), mapWidth);
            return false;
        }
    }

    heightMap = Array2D(mapWidth, mapHeight, heightData);
    return true;
}

typedef std::function<bool(int, int)> TransitionConditionFunction;
typedef std::function<bool(const Int2&)> TargetConditionFunction;

struct OpenNode
{
    Int2 coord;
    int distance;

    OpenNode(Int2 coord, int distance) : coord(coord), distance(distance) {}

    bool operator>(const OpenNode& other)
    {
        return distance > other.distance;
    }
};

int dijkstra(const Array2D<int>& heightMap, const Int2& start, TransitionConditionFunction transitionFunction, TargetConditionFunction targetFunction)
{
    const Int2 neighbourDirections[4] = { Int2::Right, Int2::Down, Int2::Left, Int2::Up };

    BinaryHeap<OpenNode, std::greater<>> openSet;
    Array2D<bool> closeSet(heightMap.getWidth(), heightMap.getHeight());

    // Push the starting node
    openSet.push(OpenNode(start, 0));

    while (!openSet.empty()) {
        OpenNode current = openSet.pop();

        // Has the current node already been visited ?
        if (closeSet[current.coord])
            continue;

        // Do we reach the target ?
        if (targetFunction(current.coord))
            return current.distance;

        // For each neighbours
        int currentHeight = heightMap[current.coord];
        for (const Int2& dir : neighbourDirections) {
            Int2 neighbourCoord = current.coord + dir;

            // Is the neihbour in range
            if (!heightMap.isInRange(neighbourCoord))
                continue;

            // Has the neihbour already been visited ?
            if (closeSet[neighbourCoord])
                continue;

            // Check the height difference
            if (!transitionFunction(currentHeight, heightMap[neighbourCoord]))
                continue;

            // Push the neighbour to the open set
            openSet.push(OpenNode(neighbourCoord, current.distance + 1));
        }

        // Set the current node as visited
        closeSet[current.coord] = true;
    }

    error("no path found");
    return INT_MAX;
}

Result Day12::runPart1() const
{
    TransitionConditionFunction transitionFunction = [](int currentHeight, int neighbourHeight) { return neighbourHeight <= currentHeight + 1; };
    TargetConditionFunction targetFunction = [this](const Int2& currentCoord) { return currentCoord == this->target; };
    return dijkstra(heightMap, start, transitionFunction, targetFunction);
}

Result Day12::runPart2() const
{
    TransitionConditionFunction transitionFunction = [](int currentHeight, int neighbourHeight) { return neighbourHeight >= currentHeight - 1; };
    TargetConditionFunction targetFunction = [this](const Int2& currentCoord) { return this->heightMap[currentCoord] == 0; };
    return dijkstra(heightMap, target, transitionFunction, targetFunction);
}

bool Day12::tryGetExpectedResultPart1(Result& outResult) const
{
    outResult = 484;
    return true;
}

bool Day12::tryGetExpectedResultPart2(Result& outResult) const
{
    outResult = 478;
    return true;
}
