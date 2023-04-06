#include "StdAfx.h"

#include "Day17.h"

#include "Result.h"
#include "Utils/Array2D.h"
#include "Utils/Exception.h"
#include "Utils/Int2.h"
#include "Utils/Log.h"

namespace
{
    struct Sequence
    {
        int32_t startIndex;
        int32_t startHeight;
        int32_t length;
        int32_t height;
        std::vector<int32_t> heightDiffSequence;
    };

    bool move(const Array2D<bool>& tower, const Day17::Rock& rock, Int2& position, Int2 direction)
    {
        for (Int2 block : rock) {
            block += position;
            block += direction;
            if (!tower.isInBounds(block) || tower[block])
                return false; // Can't move
        }

        position += direction;
        return true;
    }

    std::optional<Sequence> tryFindRepeatingSequenceInHistory(const std::vector<int32_t>& heightHistory, const std::vector<int32_t>& heightDiffHistory, int32_t sampleLength)
    {
        int32_t historySize = (int32_t)heightDiffHistory.size();
        int32_t doubleSampleLength = sampleLength * 2;
        if (historySize >= doubleSampleLength) {
            for (int32_t index = 0; index < historySize - doubleSampleLength; ++index)
            {
                // Test if the 2 squences are equals
                auto first1 = heightDiffHistory.end() - sampleLength;
                auto last1 = heightDiffHistory.end();
                auto first2 = heightDiffHistory.end() - doubleSampleLength - index;
                if (std::equal(first1, last1, first2))
                {
                    // Sequence found! Return the result
                    Sequence sequence{};
                    sequence.startIndex = (int32_t)historySize - doubleSampleLength - index;
                    sequence.startHeight = heightHistory[sequence.startIndex];
                    sequence.length = sampleLength + index;
                    sequence.height = heightHistory[sequence.startIndex + sequence.length] - sequence.startHeight;

                    // Copy the height diff sequence
                    auto begin = heightDiffHistory.begin() + sequence.startIndex;
                    auto end = begin + sequence.length;
                    sequence.heightDiffSequence.insert(sequence.heightDiffSequence.end(), begin, end);
                    assert(sequence.heightDiffSequence.size() == (size_t)sequence.length);

                    return sequence;
                }
            }
        }

        return std::nullopt;
    }

    std::optional<Sequence> findRepeatingSequenceInSimulation(const std::vector<Day17::Rock>& rocks, const std::vector<Int2>& jets, size_t rockCount, int32_t sampleLength)
    {
        // Initialize the tower
        int32_t towerHeight = 0;
        Array2D<bool> tower(7, rockCount * 4);

        // Initialize the height history
        std::vector<int32_t> heightHistory;
        std::vector<int32_t> heightDiffHistory;
        heightHistory.reserve(rockCount + 1);
        heightHistory.push_back(0);
        heightDiffHistory.reserve(rockCount);

        // Simualte rock falls until a repeating sequence is found
        size_t jetIndex = 0;
        for (size_t rockIndex = 0; rockCount; ++rockIndex)
        {
            const Day17::Rock& rock = rocks[rockIndex % rocks.size()];
            Int2 rockPosition(2, towerHeight + 3);

            while (true)
            {
                // Apply the jet move
                const Int2& jet = jets[jetIndex % jets.size()];
                move(tower, rock, rockPosition, jet);
                ++jetIndex;

                // Apply the gravity move
                if (!move(tower, rock, rockPosition, Int2::Down))
                {
                    // Write the rock in the tower
                    for (Int2 block : rock) {
                        block += rockPosition;
                        assert(tower.isInBounds(block));
                        assert(!tower[block]);
                        tower[block] = true;
                    }

                    // Update height data
                    int32_t newHeight = rockPosition.y + rock.height;
                    heightDiffHistory.push_back(newHeight - towerHeight);
                    if (newHeight > towerHeight)
                        towerHeight = newHeight;
                    heightHistory.push_back(towerHeight);

                    // Try to find a repeating sequence in the history
                    std::optional<Sequence> result = tryFindRepeatingSequenceInHistory(heightHistory, heightDiffHistory, sampleLength);
                    if (result.has_value())
                        return result;

                    break;
                }
            }
        }

        return std::nullopt;
    }

    Result simulate(const std::vector<Day17::Rock>& rocks, const std::vector<Int2>& jets, int64_t rockCount)
    {
        // Find a repeating sequence in the simulation
        std::optional<Sequence> result = findRepeatingSequenceInSimulation(rocks, jets, 5000, 100);
        if (!result.has_value()) {
            error("repeating sequence not found");
            return Result::Invalid;
        }

        // Compute the result from the sequence information
        Sequence sequence = result.value();
        int64_t sequenceCount = (rockCount - sequence.startIndex) / sequence.length;
        int64_t remaingRockCount = rockCount - sequence.startIndex - (sequenceCount * sequence.length);
        assert(remaingRockCount < sequence.length);
        int64_t height = (int64_t)sequence.startHeight + sequenceCount * sequence.height;
        size_t length = (size_t)remaingRockCount;
        for (size_t i = 0; i < length; ++i) {
            if (sequence.heightDiffSequence[i] > 0)
                height += sequence.heightDiffSequence[i];
        }

        return height;
    }
}

Day17::Rock::Rock(std::initializer_list<Int2> list)
    : height(0)
{
    size_t i = 0;
    for (Int2 block : list) {
        assert(block.x >= 0);
        assert(block.y >= 0);
        blocks[i] = block;
        if (block.y >= height)
            height = block.y + 1;
        ++i;
    }

    size = i;
}

std::array<Int2, 5>::const_iterator Day17::Rock::begin() const
{
    return blocks.begin();
}

std::array<Int2, 5>::const_iterator Day17::Rock::end() const
{
    return blocks.begin() + size;
}

void Day17::parseFile(std::ifstream& file)
{
    // Create the rocks
    rocks.push_back(Rock{ Int2(0, 0), Int2(1, 0), Int2(2, 0), Int2(3, 0) });
    rocks.push_back(Rock{ Int2(1, 0), Int2(0, 1), Int2(1, 1), Int2(2, 1), Int2(1, 2) });
    rocks.push_back(Rock{ Int2(0, 0), Int2(1, 0), Int2(2, 0), Int2(2, 1), Int2(2, 2) });
    rocks.push_back(Rock{ Int2(0, 0), Int2(0, 1), Int2(0, 2), Int2(0, 3) });
    rocks.push_back(Rock{ Int2(0, 0), Int2(1, 0), Int2(0, 1), Int2(1, 1) });

    // Get the line form the input file
    std::string line;
    if (!std::getline(file, line))
        exception("fail to get the first line");

    // Parse the jets directions
    for (char c : line) {
        switch (c) {
        case '<': jets.push_back(Int2::Left); break;
        case '>': jets.push_back(Int2::Right); break;
        default:
            exception("invalid character: {}", c);
        }
    }
}

Result Day17::runPart1() const
{
    return simulate(rocks, jets, 2022);
}

Result Day17::runPart2() const
{
    return simulate(rocks, jets, 1000000000000);
}

Result Day17::getExpectedResultPart1() const
{
    return 3219;
}

Result Day17::getExpectedResultPart2() const
{
    return 1582758620701;
}
