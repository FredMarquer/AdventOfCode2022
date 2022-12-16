#include "Day06.h"

#include <assert.h>
#include <fstream>
#include <string>

#include "Result.h"
#include "Utils/Exception.h"
#include "Utils/Log.h"

void Day06::parseFile(std::ifstream& file)
{
    std::getline(file, signal);
    if (signal.empty())
        exception("line is empty");
}

size_t getLetterIndex(char letter)
{
    if (letter < 'a' || letter > 'z') {
        error("invalid letter: ", letter);
        return 0;
    }

    return letter - 'a';
}

class LetterTracker
{
private:
    size_t markerLength;
    size_t uniqueLetterCount;
    int letterCounts[26];

public:
    LetterTracker(size_t markerLength): markerLength(markerLength), uniqueLetterCount(0), letterCounts{0} {}

    void addLetter(char letter)
    {
        // Increment the letter count
        size_t letterIndex = getLetterIndex(letter);
        ++letterCounts[letterIndex];

        // Update the unique letter count if necessary
        if (letterCounts[letterIndex] == 1)
            ++uniqueLetterCount;
        else if (letterCounts[letterIndex] == 2) {
            --uniqueLetterCount;
            assert(uniqueLetterCount >= 0);
        }
    }

    void removeLetter(char letter)
    {
        // Decrement the letter count
        size_t letterIndex = getLetterIndex(letter);
        --letterCounts[letterIndex];
        assert(letterCounts[letterIndex] >= 0);

        // Update the unique letter count if necessary
        if (letterCounts[letterIndex] == 0) {
            --uniqueLetterCount;
            assert(uniqueLetterCount >= 0);
        }
        else if (letterCounts[letterIndex] == 1)
            ++uniqueLetterCount;
    }

    bool isMarker()
    {
        return uniqueLetterCount == markerLength;
    }
};

Result findMarker(const std::string& signal, size_t markerLength)
{
    LetterTracker letterTracker = LetterTracker(markerLength);

    // Add the first N-1 letters to the tracker
    for (size_t i = 0; i < markerLength - 1; ++i)
        letterTracker.addLetter(signal[i]);

    // Process the remaining letters
    size_t size = signal.size();
    for (size_t i = markerLength - 1; i < size; ++i) {
        letterTracker.addLetter(signal[i]);

        if (letterTracker.isMarker())
            return i + 1;

        letterTracker.removeLetter(signal[i - (markerLength - 1)]);
    }

    error("marker of length {} not found", markerLength);
    return Result::Invalid;
}

Result Day06::runPart1() const
{
    return findMarker(signal, 4);
}

Result Day06::runPart2() const
{
    return findMarker(signal, 14);
}

Result Day06::getExpectedResultPart1() const
{
    return 1210;
}

Result Day06::getExpectedResultPart2() const
{
    return 3476;
}
