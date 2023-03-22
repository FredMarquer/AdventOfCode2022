#include "StdAfx.h"

#include "Day23.h"

#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/Int2.h"
#include "Utils/Rect.h"

namespace
{
    struct Elf
    {
        Int2 position;
        std::optional<Int2> proposedDestination;

        Elf(Int2 position) : position(position), proposedDestination(position) {}
    };

    struct Cell
    {
        bool hasElf;
        int proposeCount;
        bool isProposeValid;
    };

    bool hasElf(Int2 position, const Array2D<Cell>& map)
    {
        return map.isInBounds(position) && map[position].hasElf;
    }

    struct DirectionData
    {
        Int2 moveDirection;
        std::array<Int2, 3> checkDirections;

        DirectionData(Int2 moveDirection, std::array<Int2, 3> checkDirections) : moveDirection(moveDirection), checkDirections(checkDirections) {}

        bool isValid(Int2 position, const Array2D<Cell>& map) const;
    };

    bool DirectionData::isValid(Int2 position, const Array2D<Cell>& map) const
    {
        for (Int2 checkDirection : checkDirections) {
            if (hasElf(position + checkDirection, map))
                return false;
        }

        return true;
    }

    const std::array<DirectionData, 4> directionDatas{
        DirectionData(Int2(0, -1), { Int2(0, -1), Int2(1, -1), Int2(-1, -1) }),
        DirectionData(Int2(0, 1), { Int2(0, 1), Int2(1, 1), Int2(-1, 1) }),
        DirectionData(Int2(-1, 0), { Int2(-1, 0), Int2(-1, 1), Int2(-1, -1) }),
        DirectionData(Int2(1, 0), { Int2(1, 0), Int2(1, 1), Int2(1, -1) }),
    };

    bool hasAdjacentElf(Int2 position, Array2D<Cell>& map)
    {
        return
            hasElf(position + Int2(1, 1), map) ||
            hasElf(position + Int2(1, 0), map) ||
            hasElf(position + Int2(1, -1), map) ||
            hasElf(position + Int2(0, 1), map) ||
            hasElf(position + Int2(0, -1), map) ||
            hasElf(position + Int2(-1, 1), map) ||
            hasElf(position + Int2(-1, 0), map) ||
            hasElf(position + Int2(-1, -1), map);
    }

    void computeProposedDestination(Elf& elf, Array2D<Cell>& map, size_t iteration)
    {
        if (hasAdjacentElf(elf.position, map)) {
            for (size_t i = 0; i < 4; ++i) {
                const DirectionData& directionData = directionDatas[(iteration + i) % 4];
                if (directionData.isValid(elf.position, map)) {
                    Int2 proposedDestination = elf.position + directionData.moveDirection;
                    elf.proposedDestination = proposedDestination;
                    Cell& cell = map[proposedDestination];
                    assert(!cell.hasElf);
                    ++cell.proposeCount;
                    cell.isProposeValid = cell.proposeCount == 1;
                    return;
                }
            }
        }

        elf.proposedDestination = std::nullopt;
    }

    bool applyProposedDestination(Elf& elf, Array2D<Cell>& map)
    {
        if (!elf.proposedDestination.has_value())
            return false;

        Int2 proposedDestination = elf.proposedDestination.value();
        Cell& cell = map[proposedDestination];
        --cell.proposeCount;

        if (!cell.isProposeValid)
            return false;

        assert(map[elf.position].hasElf);
        map[elf.position].hasElf = false;
        elf.position = proposedDestination;
        assert(!cell.hasElf);
        cell.hasElf = true;

        return true;
    }

    struct SimulationResult
    {
        int32_t part1;
        int32_t part2;
    };

    SimulationResult simulate(std::vector<Int2> elvesPositions, size_t startingWidth, size_t startingHeight, size_t maxIteration)
    {
        size_t extend = std::min(maxIteration, elvesPositions.size() / 2);

        // Create map
        size_t width = startingWidth + (extend * 2);
        size_t height = startingHeight + (extend * 2);
        Array2D<Cell> map(width, height);

        // Create elves data
        std::vector<Elf> elves;
        elves.reserve(elvesPositions.size());
        for (Int2 pos : elvesPositions) {
            pos.x += (int32_t)extend;
            pos.y += (int32_t)extend;
            map[pos].hasElf = true;
            elves.push_back(Elf(pos));
        }

        // Simulate the elves moves
        size_t i = 0;
        for (; i < maxIteration; ++i)
        {
            for (Elf& elf : elves)
                computeProposedDestination(elf, map, i);

            bool hasMove = false;
            for (Elf& elf : elves)
                hasMove |= applyProposedDestination(elf, map);

            if (!hasMove)
                break;
        }

        // Compute the smallest rectangle
        Rect rect(elves[0].position);
        for (const Elf& elf : elves)
            rect.encapsulate(elf.position);

        // Compute and return the result
        int32_t part1 = rect.getArea() - (int32_t)elves.size();
        return SimulationResult(part1, (int32_t)i + 1);
    }
}

void Day23::parseFile(std::ifstream& file)
{
    int32_t y = 0;
    std::string line;
    while (std::getline(file, line)) {
        for (size_t x = 0; x < line.size(); ++x) {
            if (line[x] == '#')
                elvesPositions.push_back(Int2((int32_t)x, y));
        }
        ++y;
    }

    startingWidth = (int32_t)line.size();
    startingHeight = y;
}

Result Day23::runPart1() const
{
    return simulate(elvesPositions, startingWidth, startingHeight, 10).part1;
}

Result Day23::runPart2() const
{
    return simulate(elvesPositions, startingWidth, startingHeight, SIZE_MAX).part2;
}

Result Day23::getExpectedResultPart1() const
{
    return 4114;
}

Result Day23::getExpectedResultPart2() const
{
    return 970;
}
