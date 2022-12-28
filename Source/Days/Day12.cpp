#include "StdAfx.h"

#include "Day12.h"

#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/BinaryHeap.h"
#include "Utils/Exception.h"
#include "Utils/Int2.h"
#include "Utils/Log.h"

int parseHeight(char c, bool& isStart, bool& isTarget)
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
    else if (c < 'a' || c > 'z')
        exception("invalid char: {}", c);

    return c - 'a';
}

void Day12::parseFile(std::ifstream& file)
{
    size_t mapWidth = 0;
    size_t mapHeight = 0;
    std::vector<int> heightData;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            exception("line is empty");

        // Push the line into the height map
        for (size_t i = 0; i < line.size(); ++i) {
            bool isStart = false;
            bool isTarget = false;
            int cellHeight = parseHeight(line[i], isStart, isTarget);
            
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

        if (line.size() != mapWidth)
            exception("line size (= {}) doesn't match the map width (= {})", line.size(), mapWidth);
    }

    heightMap = Array2D(mapWidth, mapHeight, heightData);
}

typedef std::function<bool(int, int)> TransitionConditionFunction;
typedef std::function<bool(Int2)> TargetConditionFunction;

struct OpenNode
{
    Int2 coord;
    int distance;

    OpenNode(Int2 coord, int distance)
        : coord(coord)
        , distance(distance)
    {}

    bool operator>(const OpenNode& other)
    {
        return distance > other.distance;
    }
};

Result dijkstra(const Array2D<int>& heightMap, Int2 start, TransitionConditionFunction transitionFunction, TargetConditionFunction targetFunction)
{
    const Int2 neighbourDirections[4] = { Int2::Right, Int2::Down, Int2::Left, Int2::Up };

    BinaryHeap<OpenNode, std::greater<>> openSet;
    Array2D<bool> closeSet(heightMap.getWidth(), heightMap.getHeight());

    // Push the starting node
    openSet.push(OpenNode(start, 0));

    while (!openSet.isEmpty()) {
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
    return Result::Invalid;
}

Result Day12::runPart1() const
{
    TransitionConditionFunction transitionFunction = [](int currentHeight, int neighbourHeight) { return neighbourHeight <= currentHeight + 1; };
    TargetConditionFunction targetFunction = [this](Int2 currentCoord) { return currentCoord == this->target; };
    return dijkstra(heightMap, start, transitionFunction, targetFunction);
}

Result Day12::runPart2() const
{
    TransitionConditionFunction transitionFunction = [](int currentHeight, int neighbourHeight) { return neighbourHeight >= currentHeight - 1; };
    TargetConditionFunction targetFunction = [this](Int2 currentCoord) { return this->heightMap[currentCoord] == 0; };
    return dijkstra(heightMap, target, transitionFunction, targetFunction);
}

Result Day12::getExpectedResultPart1() const
{
    return 484;
}

Result Day12::getExpectedResultPart2() const
{
    return 478;
}
