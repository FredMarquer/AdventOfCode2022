#include "StdAfx.h"

#include "Day22.h"

#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/Exception.h"
#include "Utils/Int2.h"
#include "Utils/Parsing.h"

const Int2 directions[4] = { Int2(1, 0), Int2(0, 1), Int2(-1, 0), Int2(0, -1) };

const size_t regionSize = 50;

Day22::Tiles charToTile(char c)
{
    switch (c) {
    case ' ': return Day22::Tiles::OutOfMap;
    case '.': return Day22::Tiles::Empty;
    case '#': return Day22::Tiles::Wall;
    default:
        exception("invalid character: {}", c);
    }
}

Array2D<Day22::Tiles> parseMap(std::ifstream& file)
{
    size_t maxLineSize = 0;
    std::vector<std::string> lines;
    {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty())
                break;
            maxLineSize = std::max(maxLineSize, line.size());
            lines.push_back(std::move(line));
        }
    }

    Array2D<Day22::Tiles> map(maxLineSize, lines.size());
    for (size_t y = 0; y < lines.size(); ++y) {
        const std::string& line = lines[y];
        for (size_t x = 0; x < line.size(); ++x)
            map.at(x, y) = charToTile(line[x]);
    }

    return map;
}

std::vector<Day22::MoveInstruction> parseMoveInstructions(std::ifstream& file)
{
    std::string line;
    if (!std::getline(file, line))
        exception("fail to get line");

    std::vector<Day22::MoveInstruction> moveInstructions;

    size_t pos = 0;
    while (pos < line.size()) {
        char c = line[pos];
        if (c >= '0' && c <= '9') {
            size_t count = 1;
            while (pos + count < line.size() && line[pos + count] >= '0' && line[pos + count] <= '9')
                ++count;

            int operand;
            parse(std::string_view(line).substr(pos, count), operand);
            moveInstructions.emplace_back(Day22::MoveInstructionTypes::Move, operand);
            pos += count;
        }
        else {
            if (c == 'R')
                moveInstructions.emplace_back(Day22::MoveInstructionTypes::RotateRight);
            else if (c == 'L')
                moveInstructions.emplace_back(Day22::MoveInstructionTypes::RotateLeft);
            else
                exception("invalid character: {}", c);
            ++pos;
        }
    }

    return moveInstructions;
}

void Day22::parseFile(std::ifstream& file)
{
    map = parseMap(file);
    moveInstructions = parseMoveInstructions(file);
}

int32_t wrapValue(int32_t n, int32_t limit)
{
    return n >= 0 ? n % limit : (n % limit + limit) % limit;
}

struct CharacterState
{
    Int2 position;
    int32_t direction;

    CharacterState() : position(Int2::Zero), direction(0) {}
};

class WrapStrategy
{
public:
    virtual CharacterState wrap(CharacterState character, const Array2D<Day22::Tiles>& map) const = 0;
};

class WrapStrategyPart1 : public WrapStrategy
{
public:
    CharacterState wrap(CharacterState character, const Array2D<Day22::Tiles>& map) const override;
};

CharacterState WrapStrategyPart1::wrap(CharacterState character, const Array2D<Day22::Tiles>& map) const
{
    Int2 direction = directions[character.direction];

    do {
        character.position.x = wrapValue(character.position.x + direction.x, (int32_t)map.getWidth());
        character.position.y = wrapValue(character.position.y + direction.y, (int32_t)map.getHeight());
    } while (map[character.position] == Day22::Tiles::OutOfMap);

    return character;
}

/*
class WrapStrategyPart2 : public WrapStrategy
{
private:
    struct Neighbour
    {
        size_t regionIndex;
        Int2 fromOffset;
        std::array<std::array<int32_t, 2>, 2> transformationMatrix;
        Int2 toOffset;
        int32_t newDirection;
    };

    struct Region
    {
        bool isValid;
        std::array<Neighbour, 4> neighbours;
    };

    size_t width;
    size_t height;
    std::vector<Region> regions;

public:
    WrapStrategyPart2(const Array2D<Day22::Tiles>& map);

    CharacterState wrap(CharacterState character, const Array2D<Day22::Tiles>& map) const override;

private:
    size_t positionToRegionIndex(Int2 position) const;
};

WrapStrategyPart2::WrapStrategyPart2(const Array2D<Day22::Tiles>& map)
{
    width = map.getWidth() / regionSize;
    height = map.getHeight() / regionSize;
    regions.resize(width * height);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            size_t i = x + y * width;
            regions[i].isValid = map.at(x * regionSize, y * regionSize) != Day22::Tiles::OutOfMap;
        }
    }

    // TODO
}

CharacterState WrapStrategyPart2::wrap(CharacterState character, const Array2D<Day22::Tiles>& map) const
{
    size_t regionIndex = positionToRegionIndex(character.position);
    const Region& region = regions[regionIndex];
    const Neighbour& neighbour = region.neighbours[character.direction];

    character.position += neighbour.fromOffset;
    character.position = Int2(
        character.position.x * neighbour.transformationMatrix[0][0] + character.position.y * neighbour.transformationMatrix[0][1],
        character.position.x * neighbour.transformationMatrix[1][0] + character.position.y * neighbour.transformationMatrix[1][1]);
    character.position += neighbour.toOffset;
    assert(map[character.position] != Day22::Tiles::OutOfMap);

    character.direction = neighbour.newDirection;

    return character;
}

size_t WrapStrategyPart2::positionToRegionIndex(Int2 position) const
{
    position.x /= regionSize;
    position.y /= regionSize;
    return (size_t)position.x + (size_t)position.y * width;
}
*/

void move(CharacterState& character, int distance, const Array2D<Day22::Tiles>& map, const WrapStrategy* wrapStrategy)
{
    for (int i = 0; i < distance; ++i)
    {
        CharacterState newCharacter = character;
        newCharacter.position += directions[character.direction];

        if (!map.isInBounds(newCharacter.position) || map[newCharacter.position] == Day22::Tiles::OutOfMap)
            newCharacter = wrapStrategy->wrap(character, map);

        if (map[newCharacter.position] == Day22::Tiles::Wall)
            break;

        character = newCharacter;
    }
}

int simulate(const std::vector<Day22::MoveInstruction>& moveInstructions, const Array2D<Day22::Tiles>& map, const WrapStrategy* wrapStrategy)
{
    CharacterState character;

    // Find the starting position
    while (map[character.position] != Day22::Tiles::Empty)
        ++character.position.x;

    // Process instructions
    for (Day22::MoveInstruction instruction : moveInstructions) {
        switch (instruction.type) {
        case Day22::MoveInstructionTypes::Move:
            move(character, instruction.operand, map, wrapStrategy);
            break;
        case Day22::MoveInstructionTypes::RotateRight:
            character.direction = wrapValue(character.direction + 1, 4);
            break;
        case Day22::MoveInstructionTypes::RotateLeft:
            character.direction = wrapValue(character.direction - 1, 4);
            break;
        }
    }

    // Compute and return the result
    return (character.position.y + 1) * 1000 + (character.position.x + 1) * 4 + character.direction;
}

Result Day22::runPart1() const
{
    WrapStrategyPart1 wrapStrategy;
    return simulate(moveInstructions, map, &wrapStrategy);
}

Result Day22::runPart2() const
{
    //WrapStrategyPart2 wrapStrategy(map);
    //return simulate(moveInstructions, map, &wrapStrategy);
    return Result::Invalid;
}

Result Day22::getExpectedResultPart1() const
{
    return 77318;
}

Result Day22::getExpectedResultPart2() const
{
    return Result::Invalid;
}
