#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

struct File
{
public:
	std::string name;
	int size;

	File(std::string_view name, int size) : name(name), size(size) {}
	File(File&& other) noexcept
		: name(std::move(other.name))
		, size(other.size)
	{}
};

struct Directory
{
public:
	std::string name;
	std::vector<Directory> subDirectories;
	std::vector<File> files;
	int size;

	Directory(std::string_view name) : name(name), size(0) {}
	Directory(Directory&& other) noexcept
		: name(std::move(other.name))
		, subDirectories(std::move(other.subDirectories))
		, files(std::move(other.files))
		, size(other.size)
	{}

	void initializeSize();
	bool tryGetSubDirectoryIndex(const std::string_view& directoryName, size_t& outSubDirectoryIndex) const;
};

class Day07 : public Day
{
private:
	Directory rootDirectory;

public:
	Day07() : rootDirectory(Directory("root")) {}

	std::string getInputPath() const override { return "inputs/07.txt"; }
	bool parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
};
