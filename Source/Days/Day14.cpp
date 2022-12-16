#include "Day14.h"

#include <assert.h>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/Exception.h"
#include "Utils/Int2.h"
#include "Utils/Parsing.h"
#include "Utils/Rect.h"

const Int2 sourceCoord = Int2(500, 0);

struct Line
{
    Int2 origin;
    Int2 direction;
    int32_t distance;

    Line(const Int2& start, const Int2& end)
    {
        // Convert (start, end) into (orign, direction, distance)
        origin = start;

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

bool tryParseNextPoint(std::string_view& view, Int2& outPoint)
{
    if (view.empty())
        return false;

    // Parse x
    size_t comma = view.find_first_of(',');
    parse(view.substr(0, comma), outPoint.x);

    // Parse y
    view = view.substr(comma + 1);
    size_t space = view.find_first_of(' ');
    parse(view.substr(0, space), outPoint.y);

    view = view.substr(std::min(space + 4, view.size()));
    return true;
}

void parseLine(const std::string line, std::vector<Line>& lines, Rect& rect)
{
    std::string_view view = line;
    Int2 start;
    Int2 end;

    // Parse the first point
    if (!tryParseNextPoint(view, start))
        exception("fail to parse the first point: {}", line);

    rect.encapsulate(start);

    // Parse the next points and add lines
    while (tryParseNextPoint(view, end)) {
        lines.push_back(Line(start, end));
        rect.encapsulate(end);
        start = end;
    }
}

void Day14::parseFile(std::ifstream& file)
{
    std::vector<Line> lines;
    Rect rect = Rect::Null;

    // Generate the list of lines
    std::string line;
    while (std::getline(file, line))
        parseLine(line, lines, rect);

    // Extend the rect for part 2
    assert(rect.min.y >= 0);
    rect.min.y = 0;
    rect.max.y += 2;
    if (sourceCoord.x - rect.min.x < rect.max.y)
        rect.min.x = sourceCoord.x - rect.max.y;
    if (rect.max.x - sourceCoord.x < rect.max.y)
        rect.max.x = sourceCoord.x + rect.max.y;

    // Initialize cave data
    caveOffset = rect.min;
    caveMap = Array2D<bool>(rect.getWidth() + 1, rect.getHeight() + 1);

    // Add the lines to the map
    for (Line& line : lines) {
        Int2 coord = line.origin - caveOffset;
        for (int32_t i = 0; i <= line.distance; ++i) {
            caveMap[coord] = true;
            coord += line.direction;
        }
    }
}

bool simulateSandUnit(const Int2& source, Array2D<bool>& caveMap)
{
    assert(caveMap.isInRange(source));

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
        if (!caveMap.isInRange(coordDown))
            return false;
        if (!caveMap[coordDown]) {
            coord = coordDown;
            continue;
        }

        // Try to fall down left
        Int2 coordLeft = coordDown + Int2::Left;
        if (!caveMap.isInRange(coordLeft))
            return false;
        if (!caveMap[coordLeft]) {
            coord = coordLeft;
            continue;
        }

        // Try to fall down right
        Int2 coordRight = coordDown + Int2::Right;
        if (!caveMap.isInRange(coordRight))
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

int32_t simulate(Array2D<bool>& caveMap, const Int2& caveOffset)
{
    // Apply offset to source
    Int2 source = sourceCoord;
    source.x -= caveOffset.x;
    assert(caveMap.isInRange(source));

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

bool Day14::tryGetExpectedResultPart1(Result& outResult) const
{
    outResult = 873;
    return true;
}

bool Day14::tryGetExpectedResultPart2(Result& outResult) const
{
    outResult = 24813;
    return true;
}