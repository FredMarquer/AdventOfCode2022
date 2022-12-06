#include "Day06.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string>

int getLetterIndex(char letter)
{
    if (letter < 'a' || letter > 'z') {
        std::cout << "invalid letter : " << letter << std::endl;
        return 0;
    }

    return letter - 'a';
}

template <int N>
struct LetterTracker
{
private:
    int letterCounts[26];
    int uniqueLetterCount;

public:
    void addLetter(char letter)
    {
        // Increment the letter count
        int letterIndex = getLetterIndex(letter);
        ++letterCounts[letterIndex];

        // Update the unique letter count if necessary
        if (letterCounts[letterIndex] == 1) {
            ++uniqueLetterCount;
        }
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
        else if (letterCounts[letterIndex] == 1) {
            ++uniqueLetterCount;
        }
    }

    bool isMarker()
    {
        return uniqueLetterCount == N;
    }
};

template <int N>
int findMarker(const std::string& signal)
{
    LetterTracker<N> letterTracker = LetterTracker<N>();

    // Add the first N-1 letters to the tracker
    for (int i = 0; i < N-1; ++i) {
        letterTracker.addLetter(signal[i]);
    }

    // Process the remaining letters
    int size = signal.size();
    for (int i = N-1; i < size; ++i) {
        letterTracker.addLetter(signal[i]);

        if (letterTracker.isMarker()) {
            return i + 1;
        }

        letterTracker.removeLetter(signal[i - (N-1)]);
    }

    std::cout << "no marker found" << std::endl;
    return 0;
}

bool Day06::parseFile(std::ifstream& file)
{
    std::getline(file, signal);
    return true;
}

Result Day06::runPart1() const
{
    return findMarker<4>(signal);
}

Result Day06::runPart2() const
{
    return findMarker<14>(signal);
}