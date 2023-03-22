#include "StdAfx.h"

#include "Day20.h"

#include "Result.h"
#include "Utils/Log.h"

namespace
{
    struct Entry
    {
        int64_t value;
        int prev;
        int next;

        Entry(int64_t value, int prev, int next) : value(value), prev(prev), next(next) {}
    };

    std::vector<Entry> buildSequence(const std::vector<int>& originalSequence)
    {
        std::vector<Entry> sequence;
        sequence.reserve(originalSequence.size());

        for (int i = 0; i < (int)originalSequence.size(); ++i) {
            sequence.emplace_back(originalSequence[i], i - 1, i + 1);
        }

        sequence.front().prev = (int)sequence.size() - 1;
        sequence.back().next = 0;

        return sequence;
    }

    void mix(std::vector<Entry>& sequence)
    {
        for (size_t current = 0; current < sequence.size(); ++current)
        {
            Entry& entry = sequence[current];

            int64_t move = entry.value;
            if (move == 0)
                continue;

            sequence[entry.prev].next = entry.next;
            sequence[entry.next].prev = entry.prev;

            int after;
            if (move > 0)
            {
                move %= (sequence.size() - 1);

                after = entry.next;
                for (int i = 1; i < move; ++i)
                    after = sequence[after].next;
            }
            else
            {
                move = -move;
                move %= (sequence.size() - 1);

                after = entry.prev;
                for (int i = 0; i < move; ++i)
                    after = sequence[after].prev;
            }

            entry.prev = after;
            entry.next = sequence[after].next;
            sequence[entry.prev].next = (int)current;
            sequence[entry.next].prev = (int)current;
        }
    }

    std::optional<int> findZero(const std::vector<Entry>& sequence)
    {
        for (size_t i = 0; i < sequence.size(); ++i) {
            if (sequence[i].value == 0)
                return (int)i;
        }

        return std::nullopt;
    }

    Result decrypt(const std::vector<int>& originalSequence, int iterationCount, std::optional<int64_t> decryptionKey)
    {
        // Transform the sequence into a double linked list
        std::vector<Entry> sequence = buildSequence(originalSequence);

        // Apply the decryption key if necessary
        if (decryptionKey.has_value()) {
            for (Entry& entry : sequence)
                entry.value *= decryptionKey.value();
        }

        // Mix the sequence
        for (int i = 0; i < iterationCount; ++i)
            mix(sequence);

        // Find the value 0
        std::optional<int> zeroPos = findZero(sequence);
        if (!zeroPos.has_value()) {
            error("zero not found");
            return Result::Invalid;
        }

        // Compute the result
        int current = zeroPos.value();
        for (int i = 0; i < 1000; ++i)
            current = sequence[current].next;
        int64_t a = sequence[current].value;
        for (int i = 0; i < 1000; ++i)
            current = sequence[current].next;
        int64_t b = sequence[current].value;
        for (int i = 0; i < 1000; ++i)
            current = sequence[current].next;
        int64_t c = sequence[current].value;

        return a + b + c;
    }
}

void Day20::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line))
        sequence.push_back(std::stoi(line));
}

Result Day20::runPart1() const
{
    return decrypt(sequence, 1, std::nullopt);
}

Result Day20::runPart2() const
{
    return decrypt(sequence, 10, 811589153);
}

Result Day20::getExpectedResultPart1() const
{
    return 9945;
}

Result Day20::getExpectedResultPart2() const
{
    return 3338877775442;
}
