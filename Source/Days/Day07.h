#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "Result.h"
#include "Solver.h"

class Day07 : public Solver
{
public:
	struct File
	{
		std::string name;
		int size;

		File(std::string_view name, int size);
		File(File&& other) noexcept;
	};

	struct Directory
	{
		std::string name;
		std::vector<Directory> subDirectories;
		std::vector<File> files;
		int size;

		Directory(std::string_view name) : name(name), size(0) {}
		Directory(Directory&& other) noexcept;

		void initializeSize();
		std::optional<size_t> tryGetSubDirectoryIndex(std::string_view directoryName) const;
	};

private:
	Directory rootDirectory;

public:
	Day07() : rootDirectory(Directory("root")) {}

	const char* getInputPath() const override { return "Inputs/07.txt"; }
	void parseFile(std::ifstream& file) override;
	Result runPart1() const override;
	Result runPart2() const override;
	Result getExpectedResultPart1() const override;
	Result getExpectedResultPart2() const override;
};

