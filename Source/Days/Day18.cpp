#include "StdAfx.h"

#include "Day18.h"

#include "Result.h"
#include "Utils/Array3D.h"
#include "Utils/Exception.h"
#include "Utils/Int3.h"
#include "Utils/Parsing.h"

namespace
{
    Int3 parsePosition(std::string_view line)
    {
        auto split_view = std::views::split(line, ',');
        auto it = split_view.begin();
        auto xRange = *(it++);
        auto yRange = *(it++);
        auto zRange = *(it++);

        if (it != split_view.end())
            exception("invalid line: {}", line);

        Int3 pos{};
        parse(std::string_view{ xRange.begin(), xRange.end() }, pos.x);
        parse(std::string_view{ yRange.begin(), yRange.end() }, pos.y);
        parse(std::string_view{ zRange.begin(), zRange.end() }, pos.z);

        return pos;
    }

    void updateSurfaceCountPart1(const Array3D<bool>& grid, const Int3& pos, int32_t& surfaceArea)
    {
        if (!grid.isInBounds(pos)) {
            ++surfaceArea;
            return;
        }

        if (grid[pos])
            --surfaceArea;
        else
            ++surfaceArea;
    }

    const int32_t Empty = 0;
    const int32_t Lava = 1;
    const int32_t Air = 2;

    void processNeighbourPart2(Array3D<int32_t>& grid, std::stack<Int3>& stack, const Int3& pos, int32_t& surfaceArea)
    {
        if (!grid.isInBounds(pos))
            return;

        if (grid[pos] == Lava)
            ++surfaceArea;
        else if (grid[pos] == Empty) {
            stack.push(pos);
            grid[pos] = Air;
        }
    }
}

void Day18::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        Int3 pos = parsePosition(line);

        // Upate bounds
        if (pos.x > maxX)
            maxX = pos.x;
        if (pos.y > maxY)
            maxY = pos.y;
        if (pos.z > maxZ)
            maxZ = pos.z;

        // Add position
        positions.push_back(pos);
    }
}

Result Day18::runPart1() const
{
    Array3D<bool> grid((size_t)maxX + 1, (size_t)maxY + 1, (size_t)maxZ + 1);

    // Place all cubes, and update the surface count progressively
    int32_t surfaceArea = 0;
    for (const Int3& pos : positions) {
        assert(grid.isInBounds(pos));
        assert(!grid[pos]);
        updateSurfaceCountPart1(grid, pos + Int3::Right, surfaceArea);
        updateSurfaceCountPart1(grid, pos + Int3::Left, surfaceArea);
        updateSurfaceCountPart1(grid, pos + Int3::Up, surfaceArea);
        updateSurfaceCountPart1(grid, pos + Int3::Down, surfaceArea);
        updateSurfaceCountPart1(grid, pos + Int3::Forward, surfaceArea);
        updateSurfaceCountPart1(grid, pos + Int3::Backward, surfaceArea);
        grid[pos] = true;
    }

    return surfaceArea;
}

Result Day18::runPart2() const
{
    Array3D<int32_t> grid(maxX + 3, maxY + 3, maxZ + 3);

    // Place all cubes on the grid
    for (Int3 pos : positions) {
        pos += Int3::One;
        assert(grid.isInBounds(pos));
        assert(grid[pos] == Empty);
        grid[pos] = Lava;
    }

    // Expend air from 0,0,0 and count the exterior surface count progressively
    int32_t surfaceArea = 0;
    std::stack<Int3> stack;
    stack.push(Int3::Zero);
    grid[Int3::Zero] = Air;
    while (!stack.empty()) {
        Int3 pos = stack.top();
        stack.pop();
        processNeighbourPart2(grid, stack, pos + Int3::Right, surfaceArea);
        processNeighbourPart2(grid, stack, pos + Int3::Left, surfaceArea);
        processNeighbourPart2(grid, stack, pos + Int3::Up, surfaceArea);
        processNeighbourPart2(grid, stack, pos + Int3::Down, surfaceArea);
        processNeighbourPart2(grid, stack, pos + Int3::Forward, surfaceArea);
        processNeighbourPart2(grid, stack, pos + Int3::Backward, surfaceArea);
    }

    return surfaceArea;
}

Result Day18::getExpectedResultPart1() const
{
    return 4364;
}

Result Day18::getExpectedResultPart2() const
{
    return 2508;
}
