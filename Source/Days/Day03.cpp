#include "StdAfx.h"

#include "Day03.h"

#include "Result.h"
#include "Utils/Log.h"

namespace
{
    int itemToPriority(char letter)
    {
        if (letter >= 'a' && letter <= 'z')
            return letter - 'a' + 1;
        else if (letter >= 'A' && letter <= 'Z')
            return letter - 'A' + 27;
        
        error("invalid item: {}", letter);
        return 0;
    }

    uint64_t itemToBit(char letter)
    {
        // Use the priority as the bit position
        return (uint64_t)1 << itemToPriority(letter);
    }

    uint64_t compartmentToItemBits(std::string_view compartment)
    {
        uint64_t bits = 0;
        for (char item : compartment)
            bits |= itemToBit(item);

        return bits;
    }

    char findSharedItem(const Day03::Rucksack& rucksack)
    {
        uint64_t itemBits = compartmentToItemBits(rucksack.firstCompartment);

        for (char item : rucksack.secondCompartment) {
            uint64_t itemBit = itemToBit(item);
            if ((itemBits & itemBit) != 0)
                return item;
        }

        error("shared item not found");
        return 0;
    }

    char findBadgePriority(const Day03::Rucksack& rucksack1, const Day03::Rucksack& rucksack2, const Day03::Rucksack& rucksack3)
    {
        // Create a bit field for each rucksacks contents
        uint64_t itemBits1 = 0;
        itemBits1 |= compartmentToItemBits(rucksack1.firstCompartment);
        itemBits1 |= compartmentToItemBits(rucksack1.secondCompartment);

        uint64_t itemBits2 = 0;
        itemBits2 |= compartmentToItemBits(rucksack2.firstCompartment);
        itemBits2 |= compartmentToItemBits(rucksack2.secondCompartment);

        uint64_t itemBits3 = 0;
        itemBits3 |= compartmentToItemBits(rucksack3.firstCompartment);
        itemBits3 |= compartmentToItemBits(rucksack3.secondCompartment);

        // Find the common bit
        uint64_t badgeBit = itemBits1 & itemBits2 & itemBits3;
        if (badgeBit == 0) {
            error("shared item not found");
            return 0;
        }
        else if (((badgeBit - 1) & badgeBit) != 0) {
            error("multiple badge found");
            return 0;
        }

        // Find the position of the bit
        return (char)log2(badgeBit);
    }
}

void Day03::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        size_t compartmentSize = line.size() / 2;
        Rucksack rucksack{};
        rucksack.firstCompartment = std::string(line.begin(), line.begin() + compartmentSize);
        rucksack.secondCompartment = std::string(line.end() - compartmentSize, line.end());
        assert(rucksack.firstCompartment.size() == rucksack.secondCompartment.size());
        rucksacks.push_back(std::move(rucksack));
    }
}

Result Day03::runPart1() const
{
    // Sum the priority of all shared items
    auto view = rucksacks
        | std::ranges::views::transform(findSharedItem)
        | std::ranges::views::transform(itemToPriority);
    return std::accumulate(std::begin(view), std::end(view), 0);
}

Result Day03::runPart2() const
{
    // Sum the priority of all badges
    int sum = 0;
    size_t rucksackCount = rucksacks.size();
    for (size_t i = 0; i < rucksackCount; i += 3) {
        assert(i + 2 < rucksackCount);
        sum += findBadgePriority(rucksacks[i], rucksacks[i + 1], rucksacks[i + 2]);
    }

    return sum;
}

Result Day03::getExpectedResultPart1() const
{
    return 8252;
}

Result Day03::getExpectedResultPart2() const
{
    return 2828;
}
