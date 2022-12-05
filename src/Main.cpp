#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "Day.h"
#include "Days.h"

void parseCommandLineArguments(int argc, char* argv[], int& outDay, int& outPart)
{
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "--day") {
            if (i + 1 < argc) {
                outDay = std::stoi(argv[++i]);
                if (outDay < 0 || outDay > DayCount) {
                    std::cout << "invalid day argument '" << argv[i] << "'" << std::endl;
                    return;
                }
            }
            else {
                std::cout << "--day option requires one argument" << std::endl;
                return;
            }
        }
        else if (arg == "--part") {
            if (i + 1 < argc) {
                outPart = std::stoi(argv[++i]);
                if (outPart < 0 || outPart > 2) {
                    std::cout << "invalid part argument '" << argv[i] << "'" << std::endl;
                    return;
                }
            }
            else {
                std::cout << "--part option requires one argument" << std::endl;
                return;
            }
        }
        else {
            std::cout << "invalid option: " << arg << std::endl;
        }
    }
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

    // Process the file
    if (day->parseFile(file)) {
        bool runPart1 = part == 0 || part == 1;
        bool runPart2 = part == 0 || part == 2;

        if (runPart1) {
            Result result = day->runPart1();
            std::cout << "part 1 : " << result << std::endl;
        }

        if (runPart2) {
            Result result = day->runPart2();
            std::cout << "part 2 : " << result << std::endl;
        }
    }
    else {
        std::cout << "fail to parse the input file '" << input << "'" << std::endl;
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
    parseCommandLineArguments(argc, argv, day, part);

    if (day == 0) {
        runAllDays(part);
    }
    else {
        runDay(day, part);
    }
}