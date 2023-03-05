#include "StdAfx.h"

#include "Day24.h"

#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/BinaryHeap.h"
#include "Utils/Exception.h"
#include "Utils/Int2.h"
#include "Utils/Log.h"

std::optional<size_t> findEmptyIndex(std::string_view line)
{
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] != '#')
            return i;
    }

    return std::nullopt;
}

struct Blizzard
{
    Int2 position;
    Int2 direction;
};

std::optional<Int2> parseBlizzardDirection(char c)
{
    switch (c) {
    case '>': return Int2::Right;
    case '<': return Int2::Left;
    case '^': return Int2::Down;
    case 'v': return Int2::Up;
    default: return std::nullopt;
    }
}

void parseBlizzardLine(std::string_view line, std::vector<Blizzard>& blizzards, int32_t y)
{
    for (size_t x = 0; x < line.size(); ++x) {
        if (std::optional<Int2> dir = parseBlizzardDirection(line[x]); dir.has_value())
            blizzards.push_back(Blizzard(Int2((int32_t)x, y), dir.value()));
    }
}

std::vector<Array2D<bool>> buildGraph(std::vector<Blizzard> blizzards, int32_t width, int32_t height, Int2 startingPosition, Int2 targetPosition)
{
    size_t cycleLength;
    if (width > height && width % height == 0)
        cycleLength = width;
    else if (height > width && height % width == 0)
        cycleLength = height;
    else
        cycleLength = width * height;

    std::vector<Array2D<bool>> graph;
    graph.reserve(cycleLength);

    for (size_t i = 0; i < cycleLength; ++i)
    {
        Array2D<bool> map((size_t)width, (size_t)height);

        for (int32_t x = 0; x < width; ++x) {
            map.at(x, 0) = true;
            map.at(x, height - 1) = true;
        }

        for (int32_t y = 0; y < height; ++y) {
            map.at(0, y) = true;
            map.at(width - 1, y) = true;
        }

        for (Blizzard& blizzard : blizzards) {
            assert(blizzard.direction.isUnit());
            map[blizzard.position] = true;
            blizzard.position += blizzard.direction;
            if (blizzard.position.x <= 0)
                blizzard.position.x = width - 2;
            else if (blizzard.position.x >= width - 1)
                blizzard.position.x = 1;
            else if (blizzard.position.y <= 0)
                blizzard.position.y = height - 2;
            else if (blizzard.position.y >= height - 1)
                blizzard.position.y = 1;
        }

        map[startingPosition] = false;
        map[targetPosition] = false;

        graph.push_back(std::move(map));
    }

    return graph;
}

void Day24::parseFile(std::ifstream& file)
{
    std::string line;
    std::getline(file, line);
    std::optional<size_t> index = findEmptyIndex(line);
    if (!index.has_value())
        exception("empty index not found for starting line: {}", line);
    startingPosition = Int2((int32_t)index.value(), 0);

    int32_t y = 1;
    std::vector<Blizzard> blizzards;
    while (std::getline(file, line)) {
        if (line[1] != '#' && line[2] != '#')
            parseBlizzardLine(line, blizzards, y);
        else
            break;
        ++y;
    }

    index = findEmptyIndex(line);
    if (!index.has_value())
        exception("empty index not found for target line: {}", line);
    targetPosition = Int2((int32_t)index.value(), y);

    int32_t width = (int32_t)line.size();
    int32_t height = y + 1;
    graph = buildGraph(blizzards, width, height, startingPosition, targetPosition);
}

struct OpenNode
{
    Int2 position;
    int32_t time;
    int32_t heuristic;

    OpenNode(Int2 position, Int2 target, int time)
        : position(position)
        , time(time)
    {
        heuristic = std::abs(target.x - position.x) + std::abs(target.y - position.y) + time;
    }

    bool operator>(const OpenNode& other)
    {
        return heuristic > other.heuristic;
    }
};

std::optional<int32_t> aStar(const std::vector<Array2D<bool>>& graph, Int2 startPosition, Int2 targetPosition, int32_t startTime)
{
    const Int2 neighbourDirections[4] = { Int2::Right, Int2::Down, Int2::Left, Int2::Up };

    std::vector<Array2D<bool>> closeSet;
    closeSet.reserve(graph.size());
    for (const Array2D<bool>& map : graph)
        closeSet.emplace_back(map.getWidth(), map.getHeight());

    // Push the starting node
    BinaryHeap<OpenNode, std::greater<>> openSet;
    openSet.push(OpenNode(startPosition, targetPosition, startTime));

    while (!openSet.isEmpty()) {
        OpenNode current = openSet.pop();

        // Has the current node already been visited ?
        int32_t mapIndex = current.time % graph.size();
        Array2D<bool>& closeSubSet = closeSet[mapIndex];
        if (closeSubSet[current.position])
            continue;

        // Do we reach the target ?
        if (current.position == targetPosition)
            return current.time;

        int32_t nextTime = current.time + 1;
        int32_t nextMapIndex = nextTime % graph.size();
        const Array2D<bool>& nextMap = graph[nextMapIndex];
        const Array2D<bool>& nextCloseSubSet = closeSet[nextMapIndex];

        if (!nextMap[current.position] && !nextCloseSubSet[current.position])
            openSet.push(OpenNode(current.position, targetPosition, nextTime));

        // For each neighbours
        for (const Int2& dir : neighbourDirections) {
            Int2 neighbourCoord = current.position + dir;

            // Is the neihbour valid
            if (!nextMap.isInBounds(neighbourCoord) || nextMap[neighbourCoord])
                continue;

            // Has the neihbour already been visited ?
            if (nextCloseSubSet[neighbourCoord])
                continue;

            // Push the neighbour to the open set
            openSet.push(OpenNode(neighbourCoord, targetPosition, nextTime));
        }

        // Set the current node as visited
        closeSubSet[current.position] = true;
    }

    error("no path found");
    return std::nullopt;
}

Result Day24::runPart1() const
{
    std::optional<int32_t> result = aStar(graph, startingPosition, targetPosition, 0);
    return result.has_value() ? result.value() : Result::Invalid;
}

Result Day24::runPart2() const
{
    std::optional<int32_t> result;

    result = aStar(graph, startingPosition, targetPosition, 0);
    if (!result.has_value())
        return Result::Invalid;

    result = aStar(graph, targetPosition, startingPosition, result.value());
    if (!result.has_value())
        return Result::Invalid;

    result = aStar(graph, startingPosition, targetPosition, result.value());
    return result.has_value() ? result.value() : Result::Invalid;
}

Result Day24::getExpectedResultPart1() const
{
    return 245;
}

Result Day24::getExpectedResultPart2() const
{
    return 798;
}
