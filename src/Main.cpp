#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "Day.h"
#include "Days.h"
#include "ScopeProfiler.h"

bool parseDayOption(size_t argc, char* argv[], size_t& argIndex, int& outDay)
{
    if (argIndex + 1 >= argc) {
        std::cout << "--day option requires one argument" << std::endl;
        return false;
    }

    outDay = std::stoi(argv[++argIndex]);
    if (outDay < 0 || outDay > DayCount) {
        std::cout << "invalid day argument '" << argv[argIndex] << "'" << std::endl;
        return false;
    }

    return true;
}

bool parsePartOption(size_t argc, char* argv[], size_t& argIndex, int& outPart)
{
    if (argIndex + 1 >= argc) {
        std::cout << "--part option requires one argument" << std::endl;
        return false;
    }

    outPart = std::stoi(argv[++argIndex]);
    if (outPart < 0 || outPart > 2) {
        std::cout << "invalid part argument '" << argv[argIndex] << "'" << std::endl;
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

    std::cout << "invalid option: " << arg << std::endl;
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
    std::cout << "running day " << dayNumber << std::endl;

    // Create the day instance
    std::unique_ptr<Day> day = createDay(dayNumber);
    if (day == nullptr) {
        std::cout << "fail to create day " << dayNumber << std::endl;
        return;
    }

    // Get the input path
    std::string input = day->getInputPath();
    if (!std::filesystem::exists(input)) {
        std::cout << "input file '" << input << "' does not exist" << std::endl;
        return;
    }

    // Load the input file
    std::ifstream file(input);
    if (!file.is_open()) {
        std::cout << "fail to open the input file '" << input << "'" << std::endl;
        return;
    }

    // Parse the input file
    {
        ScopeProfiler scopeProfiler("file parsing");
        if (!day->parseFile(file)) {
            std::cout << "fail to parse the input file '" << input << "'" << std::endl;
            std::cout << "----------" << std::endl;
            file.close();
            return;
        }
    }

    bool runPart1 = part == 0 || part == 1;
    bool runPart2 = part == 0 || part == 2;

    // Run part 1 if requested
    if (runPart1) {
        ScopeProfiler scopeProfiler("part 1");
        Result result = day->runPart1();
        std::cout << "part 1 result : " << result << std::endl;
    }

    // Run part 2 if requested
    if (runPart2) {
        ScopeProfiler scopeProfiler("part 2");
        Result result = day->runPart2();
        std::cout << "part 2 result : " << result << std::endl;
    }

    file.close();
    std::cout << "----------" << std::endl;
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
        std::cout << "fail to parse command line arguments" << std::endl;
        return 1;
    }

    if (day == 0) {
        runAllDays(part);
    }
    else {
        runDay(day, part);
    }
}
