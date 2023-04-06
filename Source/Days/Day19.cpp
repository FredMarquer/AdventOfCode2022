#include "StdAfx.h"

#include "Day19.h"

#include "Result.h"
#include "Utils/Exception.h"

namespace
{
    const int32_t OreIndex = 0;
    const int32_t ClayIndex = 1;
    const int32_t ObsidianIndex = 2;
    const int32_t GeodeIndex = 3;

    struct State
    {
        int32_t timeLeft;
        std::array<int32_t, 4> resourceCount;
        std::array<int32_t, 4> robotCountPerResources;

        State(int32_t timeLeft)
            : timeLeft(timeLeft)
            , resourceCount{ 0 }
            , robotCountPerResources{ 0 }
        {}

        bool canBuildRobot(const Day19::Blueprint& blueprint, int32_t robotIndex) const
        {
            // Check if we reached the robot limit for this resource
            if (robotCountPerResources[robotIndex] >= blueprint.maxRobotPerResources[robotIndex])
                return false;

            // Check if we produce of the necessary resources
            const Day19::Cost& cost = blueprint.robotCosts[robotIndex];
            for (size_t i = 0; i < 4; ++i) {
                if (cost.costPerResources[i] > 0 && robotCountPerResources[i] == 0)
                    return false;
            }

            return true;
        }

        void collectResources(int32_t time)
        {
            for (size_t i = 0; i < 4; ++i)
                resourceCount[i] += robotCountPerResources[i] * time;
        }
    };

    int32_t computeTimeRequiredToBuildRobot(const Day19::Blueprint& blueprint, const State& state, int32_t robotIndex)
    {
        const Day19::Cost& cost = blueprint.robotCosts[robotIndex];

        int32_t timeRequired = 0;
        for (size_t i = 0; i < 4; ++i) {
            int32_t missingResources = cost.costPerResources[i] - state.resourceCount[i];
            if (missingResources <= 0)
                continue;

            if (state.robotCountPerResources[i] == 0)
                return INT32_MAX;

            int32_t t = ((missingResources - 1) / state.robotCountPerResources[i]) + 1;
            if (t > timeRequired)
                timeRequired = t;
        }

        return timeRequired + 1;
    }

    int32_t tryBuildAllRobots(const Day19::Blueprint& blueprint, const State& state);

    int32_t buildRobot(const Day19::Blueprint& blueprint, State state, int32_t robotIndex)
    {
        // Compute time required and check if the robot can be built
        int32_t timeRequired = computeTimeRequiredToBuildRobot(blueprint, state, robotIndex);
        if (timeRequired == INT32_MAX)
            return 0;

        // If we don't have enough time to build the robot,
        // collect the resource for the remaining time and return
        if (timeRequired >= state.timeLeft) {
            state.collectResources(state.timeLeft);
            return state.resourceCount[GeodeIndex];
        }

        // Collected resources
        state.collectResources(timeRequired);
        state.timeLeft -= timeRequired;

        // Spend resources
        const Day19::Cost& cost = blueprint.robotCosts[robotIndex];
        for (size_t i = 0; i < 4; ++i)
            state.resourceCount[i] -= cost.costPerResources[i];

        // Build robot
        ++state.robotCountPerResources[robotIndex];

        return tryBuildAllRobots(blueprint, state);
    }

    int32_t tryBuildAllRobots(const Day19::Blueprint& blueprint, const State& state)
    {
        // If we don't have enough time to build a robot,
        // collect the resource for the remaining time and return
        if (state.timeLeft <= 1) {
            State newState = state;
            newState.collectResources(state.timeLeft);
            return newState.resourceCount[GeodeIndex];
        }

        // Try build all robots
        int32_t maxGeodeCount = 0;
        for (int32_t i = 0; i < 4; ++i) {
            if (!state.canBuildRobot(blueprint, i))
                continue;

            int32_t geodeCount = buildRobot(blueprint, state, i);
            if (geodeCount > maxGeodeCount)
                maxGeodeCount = geodeCount;
        }

        return maxGeodeCount;
    }

    int32_t simulateBlueprint(const Day19::Blueprint& blueprint, int32_t time)
    {
        State initialState(time);
        initialState.robotCountPerResources[OreIndex] = 1;

        return tryBuildAllRobots(blueprint, initialState);
    }
}

void Day19::parseFile(std::ifstream& file)
{
    std::string line;
    std::regex regex("Blueprint ([0-9]*): Each ore robot costs ([0-9]*) ore. Each clay robot costs ([0-9]*) ore. Each obsidian robot costs ([0-9]*) ore and ([0-9]*) clay. Each geode robot costs ([0-9]*) ore and ([0-9]*) obsidian.");
    std::smatch matches;
    while (std::getline(file, line)) {
        if (!std::regex_search(line, matches, regex))
            exception("no match found for line: {}", line);

        Blueprint blueprint{ std::stoi(matches[1]) };

        // Set robots costs
        blueprint.robotCosts[OreIndex].costPerResources[OreIndex] = std::stoi(matches[2]);
        blueprint.robotCosts[ClayIndex].costPerResources[OreIndex] = std::stoi(matches[3]);
        blueprint.robotCosts[ObsidianIndex].costPerResources[OreIndex] = std::stoi(matches[4]);
        blueprint.robotCosts[ObsidianIndex].costPerResources[ClayIndex] = std::stoi(matches[5]);
        blueprint.robotCosts[GeodeIndex].costPerResources[OreIndex] = std::stoi(matches[6]);
        blueprint.robotCosts[GeodeIndex].costPerResources[ObsidianIndex] = std::stoi(matches[7]);

        // Determine the maximum number of robot needed per resources
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 4; ++j)
                blueprint.maxRobotPerResources[i] = std::max(blueprint.maxRobotPerResources[i], blueprint.robotCosts[j].costPerResources[i]);
        }

        blueprint.maxRobotPerResources[GeodeIndex] = INT32_MAX;

        blueprints.push_back(blueprint);
    }
}

Result Day19::runPart1() const
{
    int32_t result = 0;
    for (const Blueprint& blueprint : blueprints) {
        int geodeCount = simulateBlueprint(blueprint, 24);
        result += blueprint.id * geodeCount;
    }

    return result;
}

Result Day19::runPart2() const
{
    int32_t result = 1;
    for (int i = 0; i < 3; ++i)
        result *= simulateBlueprint(blueprints[i], 32);

    return result;
}

Result Day19::getExpectedResultPart1() const
{
    return 1262;
}

Result Day19::getExpectedResultPart2() const
{
    return 37191;
}
