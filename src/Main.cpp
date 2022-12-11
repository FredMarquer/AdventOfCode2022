#include <filesystem>
#include <fstream>
#include <string>

#include "Day.h"
#include "Days.h"
#include "Log.h"
#include "Result.h"
#include "ScopeProfiler.h"

bool parseDayOption(size_t argc, char* argv[], size_t& argIndex, int& outDay)
{
    if (argIndex + 1 >= argc) {
        error("--day option requires one argument");
        return false;
    }

    outDay = std::stoi(argv[++argIndex]);
    if (outDay < 0 || outDay > DayCount) {
        error("invalid day argument: {}", argv[argIndex]);
        return false;
    }

    return true;
}

bool parsePartOption(size_t argc, char* argv[], size_t& argIndex, int& outPart)
{
    if (argIndex + 1 >= argc) {
        error("--part option requires one argument");
        return false;
    }

    outPart = std::stoi(argv[++argIndex]);
    if (outPart < 0 || outPart > 2) {
        error("invalid part argument: {}", argv[argIndex]);
        return false;
    }

    return true;
}

bool parseCommandLineArguments(const std::string& arg, size_t argc, char* argv[], size_t& argIndex, int& outDay, int& outPart)
{
    if (arg == "--day")
        return parseDayOption(argc, argv, argIndex, outDay);

    if (arg == "--part")
        return parsePartOption(argc, argv, argIndex, outPart);

    error("invalid option: ", arg);
    return false;
}

bool parseCommandLineArguments(size_t argc, char* argv[], int& outDay, int& outPart)
{
    for (size_t argIndex = 1; argIndex < argc; ++argIndex) {
        std::string arg(argv[argIndex]);
        if (!parseCommandLineArguments(arg, argc, argv, argIndex, outDay, outPart))
            return false;
    }

    return true;
}

void runDay(int dayNumber, int part)
{
    log("running day {}", dayNumber);

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
    {
        profileScope("file parsing");
        if (!day->parseFile(file)) {
            error("fail to parse the input file: {}", input);
            file.close();
            return;
        }
    }

    bool runPart1 = part == 0 || part == 1;
    bool runPart2 = part == 0 || part == 2;

    // Run part 1 if requested
    if (runPart1) {
        profileScope("part 1");
        Result result = day->runPart1();
        log("part 1 result: {}", result);
    }

    // Run part 2 if requested
    if (runPart2) {
        profileScope("part 2");
        Result result = day->runPart2();
        log("part 2 result: {}", result);
    }

    file.close();
    log("----------");
}

void runAllDays(int part)
{
    for (int i = 1; i <= DayCount; ++i) {
        runDay(i, part);
    }
}

int main(int argc, char* argv[])
{
    int day = 0;
    int part = 0;
    if (!parseCommandLineArguments(argc, argv, day, part)) {
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
