#include "Day07.h"

#include <assert.h>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "Result.h"
#include "Utils/Exception.h"
#include "Utils/Parsing.h"

void Day07::Directory::initializeSize()
{
    assert(size == 0);

    for (Day07::Directory& subDirectory : subDirectories) {
        subDirectory.initializeSize(); // Initialize size recursively for all sub directories
        size += subDirectory.size;
    }

    for (const File& file : files)
        size += file.size;
}

bool Day07::Directory::tryGetSubDirectoryIndex(const std::string_view& directoryName, size_t& outSubDirectoryIndex) const
{
    for (size_t i = 0; i < subDirectories.size(); ++i) {
        if (subDirectories[i].name == directoryName) {
            outSubDirectoryIndex = i;
            return true;
        }
    }

    return false;
}

void Day07::parseFile(std::ifstream& file)
{
    // Build the file system tree
    std::vector<Directory*> currentPath;
    std::string line;
    while (std::getline(file, line)) {
        std::string_view lineView(line);

        if (line[0] == '$') {
            if (line[2] != 'c')
                continue; // Ignore the list command

            if (line[5] == '/') {
                // Return to the root directory
                currentPath.clear();
                currentPath.push_back(&rootDirectory);
                continue;
            }

            if (currentPath.empty())
                exception("current path empty");

            if (line[5] == '.') {
                // Return to the parent directory
                currentPath.pop_back();
            }
            else {
                // Go to a sub directory
                Directory* currentDirectory = currentPath.back();
                std::string_view subDirectoryName = lineView.substr(5);
                size_t subDirectoryIndex;
                if (!currentPath.back()->tryGetSubDirectoryIndex(subDirectoryName, subDirectoryIndex))
                    exception("sub directory '{}' not found", subDirectoryName);
                Directory& subDirectory = currentDirectory->subDirectories[subDirectoryIndex];
                currentPath.push_back(&subDirectory);
            }
        }
        else {
            if (currentPath.empty())
                exception("current path empty");

            if (lineView.substr(0, 3) == "dir") {
                // Add new directory to the current one
                std::string_view directoryName = lineView.substr(4);
                currentPath.back()->subDirectories.push_back(Directory(directoryName));
            }
            else {
                // Add new file to the current directory
                size_t separator = line.find(' ');
                if (separator == std::string::npos)
                    exception("separator not found for line: {}", line);

                // Parse the file size
                int fileSize = 0;
                parse(lineView.substr(0, separator), fileSize);

                std::string_view fileName = lineView.substr(separator + 1);
                currentPath.back()->files.push_back(File(fileName, fileSize));
            }
        }
    }

    // Initialize all directories' size
    rootDirectory.initializeSize();
}

void sumPart1(const Day07::Directory& directory, int& result)
{
    if (directory.size <= 100000)
        result += directory.size;

    for (const Day07::Directory& subDirectory : directory.subDirectories)
        sumPart1(subDirectory, result);
}

void findDirectorySizeToDelete(const Day07::Directory& directory, int minSpaceToDelete, int& bestDirectorySize)
{
    for (const Day07::Directory& subDirectory : directory.subDirectories) {
        if (subDirectory.size >= minSpaceToDelete)
            findDirectorySizeToDelete(subDirectory, minSpaceToDelete, bestDirectorySize);
    }

    if (directory.size < bestDirectorySize)
        bestDirectorySize = directory.size;
}

Result Day07::runPart1() const
{
    int result = 0;
    sumPart1(rootDirectory, result);
    return result;
}

Result Day07::runPart2() const
{
    // Compute the minimum space to delete
    const int totalSpace = 70000000;
    const int neededSpace = 30000000;
    const int currentAvailableSpace = rootDirectory.size;
    const int minSpaceToDelete = rootDirectory.size - (totalSpace - neededSpace);
    assert(minSpaceToDelete > 0);

    // Find best directory size to delete
    int bestDirectorySize = INT_MAX;
    findDirectorySizeToDelete(rootDirectory, minSpaceToDelete, bestDirectorySize);
    return bestDirectorySize;
}

Result Day07::getExpectedResultPart1() const
{
    return 1582412;
}

Result Day07::getExpectedResultPart2() const
{
    return 3696336;
}
