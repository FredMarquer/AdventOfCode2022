#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "Day.h"
#include "Result.h"

class Day07 : public Day
{
public:
	struct File
	{
		std::string name;
		int size;

		File(const std::string_view& name, int size) : name(name), size(size) {}
		File(File&& other) noexcept : name(std::move(other.name)), size(other.size) {}
	};

	struct Directory
	{
		std::string name;
		std::vector<Directory> subDirectories;
		std::vector<File> files;
		int size;

		Directory(const std::string_view& name) : name(name), size(0) {}
		Directory(Directory&& other) noexcept
			: name(std::move(other.name))
			, subDirectories(std::move(other.subDirectories))
			, files(std::move(other.files))
			, size(other.size)
		{}

		void initializeSize();
		std::optional<size_t> tryGetSubDirectoryIndex(const std::string_view& directoryName) const;
	};

private:
	Directory rootDirectory;

public:
	Day07() : rootDirectory(Directory("root")) {}

	std::string getInputPath() const override { return "Inputs/07.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

