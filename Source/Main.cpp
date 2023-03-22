#include "StdAfx.h"

#include <filesystem>

#include "Days.h"
#include "Result.h"
#include "Solver.h"
#include "Utils/Log.h"
#include "Utils/ScopeProfiler.h"

namespace
{
    bool parseDayOption(std::span<char*> args, size_t& argIndex, int32_t& outDay)
    {
        if (argIndex + 1 >= args.size()) {
            error("--day option requires one argument");
            return false;
        }

        outDay = (int32_t)std::atoi(args[++argIndex]);
        if (outDay < 0 || outDay > DayCount) {
            error("invalid day argument: {}", args[argIndex]);
            return false;
        }

        return true;
    }

    bool parsePartOption(std::span<char*> args, size_t& argIndex, int32_t& outPart)
    {
        if (argIndex + 1 >= args.size()) {
            error("--part option requires one argument");
            return false;
        }

        outPart = (int32_t)std::atoi(args[++argIndex]);
        if (outPart < 0 || outPart > 2) {
            error("invalid part argument: {}", args[argIndex]);
            return false;
        }

        return true;
    }

    bool parseCommandLineArguments(const char* arg, std::span<char*> args, size_t& argIndex, int32_t& outDay, int32_t& outPart)
    {
        if (strcmp(arg, "--day") == 0 ||
            strcmp(arg, "--d") == 0)
            return parseDayOption(args, argIndex, outDay);

        if (strcmp(arg, "--part") == 0 ||
            strcmp(arg, "--p") == 0)
            return parsePartOption(args, argIndex, outPart);

        error("invalid option: ", arg);
        return false;
    }

    bool parseCommandLineArguments(std::span<char*> args, int32_t& outDay, int32_t& outPart)
    {
        for (size_t argIndex = 1; argIndex < args.size(); ++argIndex) {
            if (!parseCommandLineArguments(args[argIndex], args, argIndex, outDay, outPart))
                return false;
        }

        return true;
    }

    bool processResult(Result result, Result expectedResult)
    {
        if (!result.isValid()) {
            error("part 1 returned an invalid result");
            return false;
        }

        info("part 1 result: {}", result);

        if (!expectedResult.isValid()) {
            debug("no expected result");
            return true;
        }

        if (result != expectedResult) {
            error("the result doesn't match the expected result: {}", expectedResult);
            return false;
        }

        return true;
    }

    bool runDay(int32_t dayNumber, int32_t part)
    {
        info("");
        info("---------- Day {} ----------", dayNumber);

        // Create the day instance
        std::unique_ptr<Solver> day = createDay(dayNumber);
        if (day == nullptr) {
            error("fail to create day {}", dayNumber);
            return false;
        }

        // Get the input path
        const char* input = day->getInputPath();
        if (!std::filesystem::exists(input)) {
            error("input file '{}' does not exist", input);
            return false;
        }

        try
        {
            // Load the input file
            std::ifstream file(input);
            if (!file.is_open()) {
                error("fail to open the input file: {}", input);
                return false;
            }

            // Parse the input file
            profileScope("file parsing");
            day->parseFile(file);
        }
        catch (const std::exception& e)
        {
            error("an exception has occured during file parsing:");
            error("{}", e.what());
            return false;
        }
        catch (...)
        {
            error("an unknow exception has occured during file parsing");
            return false;
        }

        bool runPart1 = part == 0 || part == 1;
        bool runPart2 = part == 0 || part == 2;

        // Run part 1 if requested
        bool part1Valid = true;
        if (runPart1)
        {
            profileScope("part 1");
            Result result = day->runPart1();
            Result expectedResult = day->getExpectedResultPart1();
            part1Valid = processResult(result, expectedResult);
        }

        // Run part 2 if requested
        bool part2Valid = true;
        if (runPart2)
        {
            profileScope("part 2");
            Result result = day->runPart2();
            Result expectedResult = day->getExpectedResultPart2();
            part2Valid = processResult(result, expectedResult);
        }

        return part1Valid && part2Valid;
    }

    bool runAllDays(int32_t part)
    {
        int32_t validDayCount = 0;

        // Run all days
        for (int32_t i = 1; i <= DayCount; ++i) {
            if (runDay(i, part))
                ++validDayCount;
        }

        // Log results
        info("");
        info("---------- Results ----------");
        info("Valid days {}/{}", validDayCount, DayCount);
        if (validDayCount < DayCount)
            error("{} day(s) failed", DayCount - validDayCount);
        info("");

        return validDayCount == DayCount;
    }
}

int main(int argc, char** argv)
{
    int32_t day = 0;
    int32_t part = 0;
    std::span<char*> args = std::span<char*>(argv, argc);
    if (!parseCommandLineArguments(args, day, part)) {
        error("fail to parse command line arguments");
        return EXIT_FAILURE;
    }

    if (day == 0) {
        if (!runAllDays(part))
            return EXIT_FAILURE;
    }
    else {
        if (!runDay(day, part))
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
