#include "StdAfx.h"

#include "Day13.h"

#include "Result.h"
#include "Utils/Exception.h"
#include "Utils/Log.h"
#include "Utils/Parsing.h"

namespace
{
    void parseListNode(std::string_view& view, Day13::Node& currentNode)
    {
        assert(currentNode.isList());

        while (!view.empty()) {
            char c = view[0];
            if (c == '[') {
                // New list node
                Day13::Node newNode;
                view = view.substr(1);
                parseListNode(view, newNode);
                assert(std::holds_alternative<std::vector<Day13::Node>>(newNode.data));
                currentNode.getList().push_back(std::move(newNode));
            }
            else if (c == ']') {
                // End current list node
                view = view.substr(1);
                return;
            }
            else if (c == ',') {
                // Go to next value
                view = view.substr(1);
                continue;
            }
            else {
                // New integer node
                int integer = 0;
                size_t separator = std::min(view.find_first_of(','), view.find_first_of(']'));
                parse(view.substr(0, separator), integer);
                currentNode.getList().push_back(integer);
                view = view.substr(separator);
            }
        }

        exception("line ended but the list as not been closed");
    }

    Day13::Node parsePacket(std::string_view view)
    {
        if (view[0] != '[')
            exception("line doesn't start with '[' : {}", view);

        // Skip the first '['
        view = view.substr(1);

        // Create and parse packet node
        Day13::Node rootNode{};
        parseListNode(view, rootNode);

        if (!view.empty())
            exception("invalid view at end of line parsing: {}", view);

        return rootNode;
    }

    size_t binarySearch(const std::vector<Day13::Node>& sortedPackets, const Day13::Node& packet)
    {
        auto dividerIt = std::ranges::lower_bound(sortedPackets, packet, std::less{});
        assert(dividerIt != sortedPackets.end());
        return dividerIt - sortedPackets.begin() + 1;
    }
}

Day13::Node::Node()
    : data(std::vector<Node>())
{}

Day13::Node::Node(int integer)
    : data(integer)
{}

bool Day13::Node::isInteger() const
{
    return std::holds_alternative<int>(this->data);
}

bool Day13::Node::isList() const
{
    return std::holds_alternative<std::vector<Day13::Node>>(this->data);
}

int Day13::Node::getInteger() const
{
    return std::get<int>(this->data);
}

std::vector<Day13::Node>& Day13::Node::getList()
{
    return std::get<std::vector<Day13::Node>>(this->data);
}

const std::vector<Day13::Node>& Day13::Node::getList() const
{
    return std::get<std::vector<Day13::Node>>(this->data);
}

std::span<const Day13::Node> Day13::Node::toSpan() const
{
    if (isInteger())
        return std::span<const Day13::Node>(this, 1);

    assert(isList());
    return getList();
}

bool Day13::Node::operator<(const Day13::Node& rhs) const
{
    if (this->isInteger() && rhs.isInteger())
        return this->getInteger() < rhs.getInteger();

    return std::ranges::lexicographical_compare(this->toSpan(), rhs.toSpan(), std::less{});
}

void Day13::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        // Parse the packet line
        Node packet = parsePacket(line);
        packets.push_back(std::move(packet));
    }

    if (packets.size() % 2 != 0)
        exception("odd number of packets (= {}), it should be even", packets.size());
}

Result Day13::runPart1() const
{
    // Sum all the valid pair indices
    size_t sum = 0;
    size_t pairCount = packets.size() / 2;
    for (size_t pairIndex = 0; pairIndex < pairCount; ++pairIndex) {
        size_t i = pairIndex * 2;
        if (packets[i] < packets[i + 1])
            sum += pairIndex + 1;
    }

    return sum;
}

Result Day13::runPart2() const
{
    // Copy the vector of packets
    std::vector<Node> sortedPackets = packets;

    // Add divider packets
    Node dividerPacket1 = parsePacket("[[2]]");
    Node dividerPacket2 = parsePacket("[[6]]");
    sortedPackets.push_back(dividerPacket1);
    sortedPackets.push_back(dividerPacket2);

    // Sort
    std::sort(sortedPackets.begin(), sortedPackets.end());

    // Find and multiply indices of divider packets
    size_t index1 = binarySearch(sortedPackets, dividerPacket1);
    size_t index2 = binarySearch(sortedPackets, dividerPacket2);
    return index1 * index2;
}

Result Day13::getExpectedResultPart1() const
{
    return 5292;
}

Result Day13::getExpectedResultPart2() const
{
    return 23868;
}
