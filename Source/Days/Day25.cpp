#include "StdAfx.h"

#include "Day25.h"

#include "Result.h"
#include "Utils/Log.h"

namespace
{
    int64_t charToDigit(char c)
    {
        switch (c) {
        case '=': return -2;
        case '-': return -1;
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        default:
            error("invalid char: {}", c);
            return 0;
        }
    }

    char digitToChar(int64_t digit)
    {
        switch (digit) {
        case -2: return '=';
        case -1: return '-';
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        default:
            error("invalid digit: {}", digit);
            return 'X';
        }
    }

    int64_t toDecimal(std::string_view view)
    {
        int64_t value = 0;
        size_t size = view.size();
        for (size_t i = 0; i < size; ++i) {
            int64_t digit = charToDigit(view[i]);
            value += digit * (int64_t)pow(5, (int64_t)(size - i - 1));
        }

        return value;
    }

    std::string toSNAFU(int64_t value)
    {
        int64_t digitIndex = 0;
        int64_t m = 0;
        while (true) {
            int64_t p = (int64_t)pow(5, digitIndex);
            m += p * 2;
            if ((m >= value))
                break;
            ++digitIndex;
        }

        std::string number;
        for (; digitIndex >= 0; --digitIndex) {
            int64_t p = (int64_t)pow(5, digitIndex);
            m -= p * 2;

            int64_t digit = value / p;
            value -= digit * p;
            if (value > 0 && value > m) {
                ++digit;
                value -= p;
            }
            else if (value < 0 && value < -m) {
                --digit;
                value += p;
            }

            char c = digitToChar(digit);
            number.push_back(c);
        }

        return number;
    }
}

void Day25::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        numbers.push_back(std::move(line));
    }
}

Result Day25::runPart1() const
{
    int64_t sum = 0;
    for (const std::string& number : numbers)
        sum += toDecimal(number);
    
    return toSNAFU(sum);
}

Result Day25::runPart2() const
{
    return Result::Invalid;
}

Result Day25::getExpectedResultPart1() const
{
    return "2-0==21--=0==2201==2";
}

Result Day25::getExpectedResultPart2() const
{
    return Result::Invalid;
}
