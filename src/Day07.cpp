#include "Day07.h"

#include <assert.h>
#include <charconv>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "Result.h"
#include "Log.h"

void sumPart1(const Directory& directory, int& result)
{
    if (directory.size <= 100000)
        result += directory.size;

    for (const Directory& subDirectory : directory.subDirectories)
        sumPart1(subDirectory, result);
}

void findDirectorySizeToDelete(const Directory& directory, int minSpaceToDelete, int& bestDirectorySize)
{
    for (const Directory& subDirectory : directory.subDirectories) {
        if (subDirectory.size >= minSpaceToDelete)
            findDirectorySizeToDelete(subDirectory, minSpaceToDelete, bestDirectorySize);
    }

    if (directory.size < bestDirectorySize)
        bestDirectorySize = directory.size;
}

void Directory::initializeSize()
{
    assert(size == 0);

    for (Directory& subDirectory : subDirectories) {
        subDirectory.initializeSize(); // Initialize size recursively for all sub directories
        size += subDirectory.size;
    }

    for (const File& file : files)
        size += file.size;
}

bool Directory::tryGetSubDirectoryIndex(const std::string_view& directoryName, size_t& outSubDirectoryIndex) const
{
    for (size_t i = 0; i < subDirectories.size(); ++i) {
        if (subDirectories[i].name == directoryName) {
            outSubDirectoryIndex = i;
            return true;
        }
    }

    return false;
}

bool Day07::parseFile(std::ifstream& file)
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

            if (currentPath.empty()) {
                error("current path empty");
                return false;
            }

            if (line[5] == '.') {
                // Return to the parent directory
                currentPath.pop_back();
            }
            else {
                // Go to a sub directory
                Directory* currentDirectory = currentPath.back();
                std::string_view subDirectoryName = lineView.substr(5);
                size_t subDirectoryIndex;
                if (currentPath.back()->tryGetSubDirectoryIndex(subDirectoryName, subDirectoryIndex)) {
                    Directory& subDirectory = currentDirectory->subDirectories[subDirectoryIndex];
                    currentPath.push_back(&subDirectory);
                }
                else {
                    error("sub directory '{}' not found", subDirectoryName);
                    return false;
                }
            }
        }
        else {
            if (currentPath.empty()) {
                error("current path empty");
                return false;
            }

            if (lineView.substr(0, 3) == "dir") {
                // Add new directory to the current one
                std::string_view directoryName = lineView.substr(4);
                currentPath.back()->subDirectories.push_back(Directory(directoryName));
            }
            else {
                // Add new file to the current directory
                size_t separator = line.find(' ');
                std::string_view sizeView = lineView.substr(0, separator);
                int fileSize = 0;
                std::from_chars(sizeView.data(), sizeView.data() + sizeView.size(), fileSize);
                assert(fileSize > 0);
                std::string_view fileName = lineView.substr(separator + 1);
                currentPath.back()->files.push_back(File(fileName, fileSize));
            }
        }
    }

    // Initialize all directories' size
    rootDirectory.initializeSize();

    return true;
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
