#include "Day06.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>

int getLetterIndex(char letter)
{
    if (letter < 'a' || letter > 'z') {
        std::cout << "invalid letter : " << letter << std::endl;
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
        int letterIndex = getLetterIndex(letter);
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
        int letterIndex = getLetterIndex(letter);
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

int findMarker(const std::string& signal, size_t markerLength)
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

    std::cout << "no marker found" << std::endl;
    return 0;
}

bool Day06::parseFile(std::ifstream& file)
{
    std::getline(file, signal);
    return !signal.empty();
}

Result Day06::runPart1() const
{
    return findMarker(signal, 4);
}

Result Day06::runPart2() const
{
    return findMarker(signal, 14);
}
