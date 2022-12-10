#include "Day03.h"

#include <assert.h>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "Result.h"
#include "Log.h"

int itemToPriority(char letter)
{
    if (letter >= 'a' && letter <= 'z')
        return letter - 'a' + 1;
    else if (letter >= 'A' && letter <= 'Z')
        return letter - 'A' + 27;
    else {
        error("invalid item: {}", letter);
        return 0;
    }
}

void addCompartmentToBitField(std::string_view compartment, int64_t& bitField)
{
    for (char item : compartment) {
        // Use the priority as the bit position
        int itemPriority = itemToPriority(item);
        bitField |= (int64_t)1 << itemPriority;
    }
}

char findSharedItem(const Rucksack& rucksack)
{
    int64_t bitField = 0;
    addCompartmentToBitField(rucksack.firstCompartment, bitField);

    for (char item : rucksack.secondCompartment) {
        int itemPriority = itemToPriority(item);
        if ((bitField & ((int64_t)1 << itemPriority)) != 0)
            return item;
    }

    error("shared item not found");
    return 0;
}

char findBadgePriority(const Rucksack& rucksack1, const Rucksack& rucksack2, const Rucksack& rucksack3)
{
    // Create a bit field for each rucksacks contents
    int64_t bitField1 = 0;
    addCompartmentToBitField(rucksack1.firstCompartment, bitField1);
    addCompartmentToBitField(rucksack1.secondCompartment, bitField1);

    int64_t bitField2 = 0;
    addCompartmentToBitField(rucksack2.firstCompartment, bitField2);
    addCompartmentToBitField(rucksack2.secondCompartment, bitField2);

    int64_t bitField3 = 0;
    addCompartmentToBitField(rucksack3.firstCompartment, bitField3);
    addCompartmentToBitField(rucksack3.secondCompartment, bitField3);

    // Find the common bit
    int64_t badgeBit = bitField1 & bitField2 & bitField3;
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

bool Day03::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        int compartmentSize = line.size() / 2;
        Rucksack rucksack = Rucksack();
        rucksack.firstCompartment = std::string(line.begin(), line.begin() + compartmentSize);
        rucksack.secondCompartment = std::string(line.end() - compartmentSize, line.end());
        assert(rucksack.firstCompartment.size() == rucksack.secondCompartment.size());
        rucksacks.push_back(std::move(rucksack));
    }

    return true;
}

Result Day03::runPart1() const
{
    // Sum the priority of all shared items
    int sum = 0;
    for (const Rucksack& rucksack : rucksacks) {
        char sharedItem = findSharedItem(rucksack);
        sum += itemToPriority(sharedItem);
    }

    return sum;
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
