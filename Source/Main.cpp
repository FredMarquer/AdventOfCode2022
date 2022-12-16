#include <exception>
#include <filesystem>
#include <fstream>
#include <span>
#include <string>

#include "Day.h"
#include "Days.h"
#include "Result.h"
#include "Utils/Log.h"
#include "Utils/ScopeProfiler.h"

bool parseDayOption(std::span<char*> args, size_t& argIndex, int& outDay)
{
    if (argIndex + 1 >= args.size()) {
        error("--day option requires one argument");
        return false;
    }

    outDay = std::stoi(args[++argIndex]);
    if (outDay < 0 || outDay > DayCount) {
        error("invalid day argument: {}", args[argIndex]);
        return false;
    }

    return true;
}

bool parsePartOption(std::span<char*> args, size_t& argIndex, int& outPart)
{
    if (argIndex + 1 >= args.size()) {
        error("--part option requires one argument");
        return false;
    }

    outPart = std::atoi(args[++argIndex]);
    if (outPart < 0 || outPart > 2) {
        error("invalid part argument: {}", args[argIndex]);
        return false;
    }

    return true;
}

bool parseCommandLineArguments(const char* arg, std::span<char*> args, size_t& argIndex, int& outDay, int& outPart)
{
    if (arg == "--day")
        return parseDayOption(args, argIndex, outDay);

    if (arg == "--part")
        return parsePartOption(args, argIndex, outPart);

    error("invalid option: ", arg);
    return false;
}

bool parseCommandLineArguments(std::span<char*> args, int& outDay, int& outPart)
{
    for (size_t argIndex = 1; argIndex < args.size(); ++argIndex) {
        if (!parseCommandLineArguments(args[argIndex], args, argIndex, outDay, outPart))
            return false;
    }

    return true;
}

void runDay(int dayNumber, int part)
{
    log("");
    log("---------- Day {} ----------", dayNumber);

    // Create the day instance
    std::unique_ptr<Day> day = createDay(dayNumber);
    if (day == nullptr) {
        error("fail to create day {}", dayNumber);
        return;
    }

    // Get the input path
    std::string input = day->getInputPath();
    if (!std::filesystem::exists(input)) {
        error("input file '{}' does not exist", input);
        return;
    }

    // Load the input file
    std::ifstream file(input);
    if (!file.is_open()) {
        error("fail to open the input file: {}", input);
        return;
    }

    // Parse the input file
    try
    {
        profileScope("file parsing");
        day->parseFile(file);
    }
    catch (std::exception& e)
    {
        error("an exception has occured during file parsing:");
        error("{}", e.what());
        file.close();
        return;
    }
    catch (...)
    {
        error("an unknow exception has occured during file parsing");
        file.close();
        return;
    }

    file.close();

    bool runPart1 = part == 0 || part == 1;
    bool runPart2 = part == 0 || part == 2;

    // Run part 1 if requested
    if (runPart1) {
        profileScope("part 1");

        Result result = day->runPart1();
        if (result.isValid()) {
            log("part 1 result: {}", result);

            Result expectedResult = day->getExpectedResultPart1();
            if (expectedResult.isValid()) {
                if (result != expectedResult)
                    error("the result doesn't match the expected result: {}", expectedResult);
            }
            else
                debug("no expected result");
        }
        else
            error("part 1 returned an invalid result");
    }

    // Run part 2 if requested
    if (runPart2) {
        profileScope("part 2");

        Result result = day->runPart2();
        if (result.isValid()) {
            log("part 2 result: {}", result);

            Result expectedResult = day->getExpectedResultPart2();
            if (expectedResult.isValid()) {
                if (result != expectedResult)
                    error("the result doesn't match the expected result: {}", expectedResult);
            }
            else
                debug("no expected result");
        }
        else
            error("part 2 returned an invalid result");
    }
}

void runAllDays(int part)
{
    for (int i = 1; i <= DayCount; ++i) {
        runDay(i, part);
    }
}

int main(int argc, char** argv)
{
    int day = 0;
    int part = 0;
    std::span<char*> args = std::span<char*>(argv, argc);
    if (!parseCommandLineArguments(args, day, part)) {
        error("fail to parse command line arguments");
        return 1;
    }

    if (day == 0) {
        runAllDays(part);
    }
    else {
        runDay(day, part);
    }
}
