#include "StdAfx.h"

#include "Day16.h"

#include "Result.h"
#include "Utils/BinaryHeap.h"
#include "Utils/Exception.h"
#include "Utils/Log.h"

namespace
{
    void parseConnectedValves(std::string_view view, std::vector<Day16::ConnectedValve>& connectedValves)
    {
        constexpr std::string_view delim{ ", " };

        for (const auto range : std::views::split(view, delim))
            connectedValves.push_back(Day16::ConnectedValve(std::string_view{ range.begin(), range.end() }));
    }

    template<size_t CharacterCount>
    struct State
    {
        int32_t timeLeft;
        int32_t remainingFlowRate;
        int32_t pressureReleased;
        std::array<size_t, CharacterCount> characterValveIndices;
        uint64_t openValveBits;

        State(size_t valveIndex, int32_t timeLeft, int32_t remainingFlowRate);

        struct MoveIterator
        {
        private:
            const std::vector<Day16::Valve>& valves;
            const State<CharacterCount>& previousState;
            State<CharacterCount> nextState;
            std::array<int32_t, CharacterCount> characterMoveIndices;
            bool isFirstIteration;

        public:
            MoveIterator(const std::vector<Day16::Valve>& valves, const State<CharacterCount>& state);

            std::optional<const State<CharacterCount>*> next();

        private:
            bool nextMoveCombination();
            bool applyNextState();
        };

        MoveIterator availableMoves(const std::vector<Day16::Valve>& valves) const;
    };

    template<size_t CharacterCount>
    State<CharacterCount>::State(size_t valveIndex, int32_t timeLeft, int32_t remainingFlowRate)
        : timeLeft(timeLeft)
        , remainingFlowRate(remainingFlowRate)
        , pressureReleased(0)
        , openValveBits(0)
    {
        characterValveIndices.fill(valveIndex);
    }

    template<size_t CharacterCount>
    State<CharacterCount>::MoveIterator State<CharacterCount>::availableMoves(const std::vector<Day16::Valve>& valves) const
    {
        return MoveIterator(valves, *this);
    }

    template<size_t CharacterCount>
    State<CharacterCount>::MoveIterator::MoveIterator(const std::vector<Day16::Valve>& valves, const State<CharacterCount>& state)
        : valves(valves)
        , previousState(state)
        , nextState(state)
        , isFirstIteration(true)
    {
        characterMoveIndices.fill(-1);
    }

    template<size_t CharacterCount>
    std::optional<const State<CharacterCount>*> State<CharacterCount>::MoveIterator::next()
    {
        // Return the first move combination if valid
        if (isFirstIteration) {
            isFirstIteration = false;
            if (applyNextState())
                return &nextState;
        }

        // Find and return the next valid move combination
        while (nextMoveCombination()) {
            if (applyNextState())
                return &nextState;
        }

        return std::nullopt;
    }

    template<size_t CharacterCount>
    bool State<CharacterCount>::MoveIterator::nextMoveCombination()
    {
        for (size_t characterIndex = 0; characterIndex < CharacterCount; ++characterIndex)
        {
            int32_t& moveIndex = characterMoveIndices[characterIndex];
            ++moveIndex;

            size_t valveIndex = previousState.characterValveIndices[characterIndex];
            const Day16::Valve& valve = valves[valveIndex];
            if (moveIndex < (int32_t)valve.connectedValves.size())
                return true;
            else
                moveIndex = -1;
        }

        return false;
    }

    template<size_t CharacterCount>
    bool State<CharacterCount>::MoveIterator::applyNextState()
    {
        nextState = previousState;
        --nextState.timeLeft;

        for (size_t characterIndex = 0; characterIndex < CharacterCount; ++characterIndex)
        {
            size_t valveIndex = nextState.characterValveIndices[characterIndex];
            const Day16::Valve& valve = valves[valveIndex];
            int32_t moveIndex = characterMoveIndices[characterIndex];
            if (moveIndex >= 0) {
                // Move to the next valve
                nextState.characterValveIndices[characterIndex] = valve.connectedValves[moveIndex].index;
            }
            else {
                // Try open the valve
                bool canOpenValve = valve.flowRate > 0 && (nextState.openValveBits & valve.bit) == 0;
                if (!canOpenValve)
                    return false;
                nextState.pressureReleased += valve.flowRate * nextState.timeLeft;
                nextState.remainingFlowRate -= valve.flowRate;
                nextState.openValveBits |= valve.bit;
            }
        }

        return true;
    }

    template<size_t CharacterCount>
    struct OpenNode
    {
        State<CharacterCount> state;
        int32_t heuristic;

        OpenNode(const std::vector<Day16::Valve>& valves, const State<CharacterCount>& state);

        inline bool operator<(const OpenNode& other) const;
    };

    template<size_t CharacterCount>
    OpenNode<CharacterCount>::OpenNode(const std::vector<Day16::Valve>& valves, const State<CharacterCount>& state) : state(state)
    {
        heuristic = state.pressureReleased;

        if (state.remainingFlowRate == 0)
            return;

        // Open the remaining valves in the best possible order and in the shortest time possible (assuming only 1 move between valves)
        size_t valveIndex = 0;
        int32_t timeLeft = state.timeLeft - 1;
        while (timeLeft > 0)
        {
            for (size_t characterIndex = 0; characterIndex < CharacterCount; ++characterIndex) {
                while ((state.openValveBits & ((uint64_t)1 << valveIndex)) != 0)
                    ++valveIndex;
                const Day16::Valve& valve = valves[valveIndex];
                if (valve.flowRate == 0)
                    return;
                heuristic += valve.flowRate * timeLeft;
                ++valveIndex;
            }

            timeLeft -= 2;
        }
    }

    template<size_t CharacterCount>
    bool OpenNode<CharacterCount>::operator<(const OpenNode& other) const
    {
        return heuristic < other.heuristic;
    }

    struct CloseNode
    {
        uint64_t characterValveBits; // This bit field works only for 1 or 2 characters.
        uint64_t openValveBits;

        template<size_t CharacterCount>
        CloseNode(const State<CharacterCount>& state);

        inline bool operator==(const CloseNode& other) const;
    };

    template<size_t CharacterCount>
    CloseNode::CloseNode(const State<CharacterCount>& state)
        : characterValveBits(0)
        , openValveBits(state.openValveBits)
    {
        for (size_t characterValveIndex : state.characterValveIndices)
            characterValveBits |= (uint64_t)1 << characterValveIndex;
    }

    bool CloseNode::operator==(const CloseNode& other) const
    {
        return
            characterValveBits == other.characterValveBits &&
            openValveBits == other.openValveBits;
    }

    struct CloseNodeHash
    {
        inline size_t operator()(const CloseNode& node) const noexcept
        {
            size_t hash1 = std::hash<uint64_t>()(node.characterValveBits);
            size_t hash2 = std::hash<uint64_t>()(node.openValveBits);
            return hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
        }
    };

    template<size_t CharacterCount>
    Result aStar(const std::vector<Day16::Valve>& valves, const State<CharacterCount>& initialState)
    {
        // Initialize the open set
        BinaryHeap<OpenNode<CharacterCount>, std::less<>> openSet(2500000);
        openSet.push(OpenNode<CharacterCount>(valves, initialState));

        // Initialize the close set
        std::unordered_set<CloseNode, CloseNodeHash> closeSet(2500000);

        // A* loop
        while (!openSet.isEmpty())
        {
            State<CharacterCount> currentState = openSet.pop().state;

            if (currentState.timeLeft <= 0 ||
                currentState.remainingFlowRate <= 0)
                return currentState.pressureReleased; // Solution found

            if (closeSet.contains(currentState))
                continue;

            // Iterate through all possible moves combinations
            typename State<CharacterCount>::MoveIterator moveIterator = currentState.availableMoves(valves);
            while (true) {
                std::optional<const State<CharacterCount>*> nextState = moveIterator.next();
                if (!nextState.has_value())
                    break;

                const State<CharacterCount>& newState = *(nextState.value());
                if (closeSet.contains(newState))
                    continue;

                openSet.push(OpenNode<CharacterCount>(valves, newState));
            }

            closeSet.insert(currentState);
        }

        error("no solution found");
        return Result::Invalid;
    }
}

Day16::ConnectedValve::ConnectedValve(std::string_view name)
    : name(std::string(name))
    , index(0)
{}

Day16::Valve::Valve(std::string&& name, int flowRate)
    : name(std::move(name))
    , index(0)
    , bit(0)
    , flowRate(flowRate)
{}

void Day16::parseFile(std::ifstream& file)
{
    const std::string startingValveName = "AA";

    std::unordered_map<std::string, size_t> valveNameToIndex;

    std::string line;
    std::regex regex("Valve ([A-Z]{2}) has flow rate=([0-9]+); tunnels? leads? to valves? (.*)");
    std::smatch matches;
    while (std::getline(file, line))
    {
        if (!std::regex_search(line, matches, regex))
            exception("no match found for line: {}", line);

        // Get regex captures
        std::string name = matches[1];
        int flowRate = std::stoi(matches[2]);
        std::string connectedValves = matches[3];

        // Create the valve
        Valve valve(std::move(name), flowRate);
        parseConnectedValves(connectedValves, valve.connectedValves);
        valves.push_back(std::move(valve));

        // Update the total flow rate
        totalFlowRate += flowRate;
    }

    // Sort the valve to help compute the A* heuristic faster
    std::ranges::sort(valves, std::ranges::greater{}, &Day16::Valve::flowRate);

    // Set valve indices
    for (size_t valveIndex = 0; valveIndex < valves.size(); ++valveIndex) {
        Valve& valve = valves[valveIndex];
        valve.index = valveIndex;
        valve.bit = (uint64_t)1 << valveIndex;
        valveNameToIndex.insert({ valve.name, valveIndex });
    }

    // Retrieve connected valve indices
    for (Valve& valve : valves) {
        for (ConnectedValve& connectedValve : valve.connectedValves)
            connectedValve.index = valveNameToIndex.at(connectedValve.name);
    }

    startingValveIndex = valveNameToIndex.at(startingValveName);

    if (valves.size() > 64)
        exception("too many vavles: {}", valves.size());
}

Result Day16::runPart1() const
{
    State<1> initialState(startingValveIndex, 30, totalFlowRate);
    return aStar(valves, initialState);
}

Result Day16::runPart2() const
{
    State<2> initialState(startingValveIndex, 26, totalFlowRate);
    return aStar(valves, initialState);
}

Result Day16::getExpectedResultPart1() const
{
    return 1873;
}

Result Day16::getExpectedResultPart2() const
{
    return 2425;
}
