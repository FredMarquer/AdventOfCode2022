#include "Day15.h"

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

#include "Result.h"
#include "Utils/Exception.h"
#include "Utils/Int2.h"
#include "Utils/Log.h"
#include "Utils/Parsing.h"
#include "Utils/Range.h"

void Day15::parseFile(std::ifstream& file)
{
    std::string line;
    std::regex regex("Sensor at x=(-?[0-9]*), y=(-?[0-9]*): closest beacon is at x=(-?[0-9]*), y=(-?[0-9]*)");
    std::smatch matches;
    while (std::getline(file, line)) {
        if (!std::regex_search(line, matches, regex))
            exception("no match found for line: {}", line);
        int sensorX = std::stoi(matches[1]);
        int sensorY = std::stoi(matches[2]);
        int beaconX = std::stoi(matches[3]);
        int beaconY = std::stoi(matches[4]);
        Report report(Int2(sensorX, sensorY), Int2(beaconX, beaconY));
        reports.push_back(report);
    }
}

std::optional<Range> tryGetRangeAtY(const Day15::Report& report, int32_t y, bool excludeBeacon)
{
    int32_t beaconDistance = std::abs(report.beacon.x - report.sensor.x) + std::abs(report.beacon.y - report.sensor.y);
    int32_t distanceFromY = std::abs(y - report.sensor.y);

    // Check if the report intersect with the raw y
    int32_t delta = beaconDistance - distanceFromY;
    if (delta < 0)
        return std::nullopt;

    Range range;
    range.min = report.sensor.x - delta;
    range.max = report.sensor.x + delta + 1;
    assert(range.getSize() >= 1);

    // Remove the beacon position from the range if necessary
    if (excludeBeacon && report.beacon.y == y) {
        if (range.getSize() == 1) {
            assert(report.beacon.x == range.min);
            return false;
        }

        if (report.beacon.x == range.min)
            ++range.min;
        else {
            assert(report.beacon.x == (range.max - 1));
            --range.max;
        }
    }

    return range;
}

void getRangesAtY(const std::vector<Day15::Report>& reports, int32_t y, bool excludeBeaconCoord, std::vector<Range>& outRanges)
{
    assert(outRanges.empty());

    // Compute the range at y for each reports
    for (const Day15::Report& report : reports) {
        std::optional<Range> range = tryGetRangeAtY(report, y, excludeBeaconCoord);
        if (range.has_value())
            outRanges.push_back(range.value());
    }

    // Merge overlapping/touching ranges
    for (size_t i = 0; i < outRanges.size(); ++i) {
        Range& range = outRanges[i];
        bool hasChanged = false;
        do {
            hasChanged = false;
            for (size_t j = outRanges.size() - 1; j > i; --j) {
                const Range& other = outRanges[j];
                if (range.touch(other)) {
                    range.encapsulate(other);
                    outRanges.erase(outRanges.begin() + j);
                    hasChanged = true;
                }
            }
        } while (hasChanged);
    }
}

Result Day15::runPart1() const
{
    std::vector<Range> ranges;
    getRangesAtY(reports, 2000000, true, ranges);

    // Sum ranges size
    int32_t sum = 0;
    for (const Range& range : ranges)
        sum += range.getSize();

    return sum;
}

Result Day15::runPart2() const
{
    const int32_t coordMax = 4000000;
    const Range bounds = Range(0, coordMax + 1);

    std::vector<Range> ranges;
    for (int32_t y = 0; y <= coordMax; ++y)
    {
        getRangesAtY(reports, y, false, ranges);

        // Clamp ranges between 0 and max
        for (int i = ranges.size() - 1; i >= 0; --i) {
            Range& range = ranges[i];
            if (range.overlap(bounds))
                range.clamp(bounds);
            else {
                std::swap(ranges[i], ranges[ranges.size() - 1]);
                ranges.pop_back();
            }
        }

        assert(ranges.size() == 1 || ranges.size() == 2);
        bool isBeaconHere = ranges.size() > 1 || ranges[0].getSize() <= coordMax;
        if (isBeaconHere) {
            // Find x
            int32_t x = 0;
            if (ranges.size() == 1) {
                const Range& range = ranges[0];
                if (range.min > 0)
                    x = 0;
                else
                    x = coordMax;
            }
            else {
                const Range& firstRange = ranges[0];
                if (firstRange.min == 0)
                    x = firstRange.max;
                else
                    x = firstRange.min - 1;
            }

            return ((int64_t)x * coordMax) + y;
        }

        ranges.clear();
    }

    error("solution not found");
    return Result::Invalid;
}

Result Day15::getExpectedResultPart1() const
{
    return 4827924;
}

Result Day15::getExpectedResultPart2() const
{
    return 12977110973564;
}
