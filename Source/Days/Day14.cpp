#include "StdAfx.h"

#include "Day14.h"

#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/Exception.h"
#include "Utils/Int2.h"
#include "Utils/Parsing.h"
#include "Utils/Rect.h"

const Int2 sourceCoord = Int2(500, 0);

struct Line
{
    Int2 start;
    Int2 end;
    Int2 direction;
    int32_t distance;

    Line(Int2 start, Int2 end)
        : start(start)
        , end(end)
    {
        // Convert (start, end) into (orign, direction, distance)
        Int2 delta = end - start;
        if (delta.x > 0) {
            assert(delta.y == 0);
            direction = Int2::Right;
            distance = delta.x;
        }
        else if (delta.x < 0) {
            assert(delta.y == 0);
            direction = Int2::Left;
            distance = -delta.x;
        }
        else if (delta.y > 0) {
            direction = Int2::Up;
            distance = delta.y;
        }
        else {
            assert(delta.y < 0);
            direction = Int2::Down;
            distance = -delta.y;
        }
    }
};

std::optional<Int2> tryParseNextPoint(std::string_view& view)
{
    if (view.empty())
        return std::nullopt;

    Int2 point;

    // Parse x
    size_t comma = view.find_first_of(',');
    parse(view.substr(0, comma), point.x);

    // Parse y
    view = view.substr(comma + 1);
    size_t space = view.find_first_of(' ');
    parse(view.substr(0, space), point.y);

    view = view.substr(std::min(space + 4, view.size()));
    return point;
}

void parseLine(const std::string& strLine, std::vector<Line>& lines)
{
    std::string_view view = strLine;

    // Parse the first point
    std::optional<Int2> start = tryParseNextPoint(view);
    if (!start.has_value())
        exception("fail to parse the first point: {}", view);
    
    // Parse the next points and add lines
    std::optional<Int2> end;
    while ((end = tryParseNextPoint(view)).has_value()) {
        lines.push_back(Line(start.value(), end.value()));
        start = end;
    }
}

void Day14::parseFile(std::ifstream& file)
{
    // Generate the list of lines
    std::string lineStr;
    std::vector<Line> lines;
    while (std::getline(file, lineStr))
        parseLine(lineStr, lines);

    // Compute the bounding rect
    Rect rect = Rect(lines[0].start);
    for (const Line& line : lines) {
        rect.encapsulate(line.start);
        rect.encapsulate(line.end);
    }

    // Extend the rect for part 2
    assert(rect.min.y >= 0);
    rect.min.y = 0;
    rect.max.y += 2;
    rect.encapsulateX(Range(sourceCoord.x - rect.max.y, sourceCoord.x + rect.max.y));

    // Initialize cave data
    caveOffset = rect.min;
    caveMap = Array2D<bool>(rect.getWidth(), rect.getHeight());

    // Add the lines to the map
    for (Line& line : lines) {
        Int2 coord = line.start - caveOffset;
        for (int32_t i = 0; i <= line.distance; ++i) {
            caveMap[coord] = true;
            coord += line.direction;
        }
    }
}

bool simulateSandUnit(Int2 source, Array2D<bool>& caveMap)
{
    assert(caveMap.isInBounds(source));

    if (caveMap[source])
    {
        return false;
    }

    // Simulate the sand unit until rest or reaching the bound of the map
    Int2 coord = source;
    while (true)
    {
        // Try to fall down
        Int2 coordDown = coord + Int2::Up; // The map is inverted on the Y axis.
        if (!caveMap.isInBounds(coordDown))
            return false;
        if (!caveMap[coordDown]) {
            coord = coordDown;
            continue;
        }

        // Try to fall down left
        Int2 coordLeft = coordDown + Int2::Left;
        if (!caveMap.isInBounds(coordLeft))
            return false;
        if (!caveMap[coordLeft]) {
            coord = coordLeft;
            continue;
        }

        // Try to fall down right
        Int2 coordRight = coordDown + Int2::Right;
        if (!caveMap.isInBounds(coordRight))
            return false;
        if (!caveMap[coordRight]) {
            coord = coordRight;
            continue;
        }

        // Rest
        assert(!caveMap[coord]);
        caveMap[coord] = true;
        return true;
    }

    return false;
}

int32_t simulate(Array2D<bool>& caveMap, Int2 caveOffset)
{
    // Apply offset to source
    Int2 source = sourceCoord;
    source.x -= caveOffset.x;
    assert(caveMap.isInBounds(source));

    // Simulate sand units until the state is stable
    int32_t sandUnitCount = 0;
    while (simulateSandUnit(source, caveMap)) {
        ++sandUnitCount;
    }

    return sandUnitCount;
}

Result Day14::runPart1() const
{
    Array2D<bool> workingCaveMap = caveMap;

    return simulate(workingCaveMap, caveOffset);
}

Result Day14::runPart2() const
{
    Array2D<bool> workingCaveMap = caveMap;
    workingCaveMap.fillRaw(workingCaveMap.getHeight() - 1, true);

    return simulate(workingCaveMap, caveOffset);
}

Result Day14::getExpectedResultPart1() const
{
    return 873;
}

Result Day14::getExpectedResultPart2() const
{
    return 24813;
}
