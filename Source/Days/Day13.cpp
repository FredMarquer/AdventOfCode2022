#include "Day13.h"

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "Result.h"
#include "Utils/Exception.h"
#include "Utils/Log.h"
#include "Utils/Parsing.h"

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
            int integer;
            size_t separator = std::min(view.find_first_of(','), view.find_first_of(']'));
            parse(view.substr(0, separator), integer);
            currentNode.getList().push_back(integer);
            view = view.substr(separator);
        }
    }

    exception("line ended but the list as not been closed");
}

Day13::Packet parsePacket(const std::string& line)
{
    std::string_view view = line;

    if (view[0] != '[')
        exception("line doesn't start with '[' : {}", view);

    // Skip the first '['
    view = view.substr(1);

    // Create and parse packet node
    Day13::Packet packet;
    packet.line = line;
    parseListNode(view, packet.rootNode);

    if (!view.empty())
        exception("invalid view at end of line parsing: {}", view);
    
    return packet;
}

void Day13::parseFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        // Parse the packet line
        Packet packet = parsePacket(line);
        packets.push_back(std::move(packet));
    }

    if (packets.size() % 2 != 0)
        exception("odd number of packets (= {}), it should be even", packets.size());
}

struct List
{
    const Day13::Node* ptr;
    size_t count;

    List(const Day13::Node& node)
    {
        if (node.isInteger()) {
            ptr = &node;
            count = 1;
        }
        else {
            assert(node.isList());
            const std::vector<Day13::Node>& list = node.getList();
            ptr = list.data();
            count = list.size();
        }
    }
};

// Foward declare
int compareNodes(const Day13::Node& lhs, const Day13::Node& rhs);

int compareLists(const List& lhs, const List& rhs)
{
    size_t count = std::min(lhs.count, rhs.count);
    for (size_t i = 0; i < count; ++i) {
        int result = compareNodes(lhs.ptr[i], rhs.ptr[i]);
        if (result != 0)
            return result;
    }

    return lhs.count - rhs.count;
}

int compareNodes(const Day13::Node& lhs, const Day13::Node& rhs)
{
    if (lhs.isInteger() && rhs.isInteger())
        return lhs.getInteger() - rhs.getInteger();

    return compareLists(lhs, rhs);
}

bool Day13::Packet::operator<(const Day13::Packet& rhs) const
{
    return compareNodes(rootNode, rhs.rootNode) < 0;
}

Result Day13::runPart1() const
{
    // Sum all the valid pair indices
    size_t sum = 0;
    size_t pairCount = packets.size() / 2;
    for (size_t pairIndex = 0; pairIndex < pairCount; ++pairIndex) {
        size_t i = pairIndex * 2;
        size_t j = i + 1;
        if (compareNodes(packets[i].rootNode, packets[j].rootNode) < 0)
            sum += pairIndex + 1;
    }

    return sum;
}

Result Day13::runPart2() const
{
    const std::string divider1 = "[[2]]";
    const std::string divider2 = "[[6]]";

    // Copy the vector of packets
    std::vector<Packet> sortedPackets = packets;

    // Add divider packets
    Packet dividerPacket1 = parsePacket(divider1);
    Packet dividerPacket2 = parsePacket(divider2);
    sortedPackets.push_back(std::move(dividerPacket1));
    sortedPackets.push_back(std::move(dividerPacket2));

    // Sort
    std::sort(sortedPackets.begin(), sortedPackets.end());

    // Find and multiply indices of divider packets
    auto dividerIterator1 = std::find(sortedPackets.begin(), sortedPackets.end(), divider1);
    auto dividerIterator2 = std::find(sortedPackets.begin(), sortedPackets.end(), divider2);
    size_t dividerIndex1 = dividerIterator1 - sortedPackets.begin() + 1;
    size_t dividerIndex2 = dividerIterator2 - sortedPackets.begin() + 1;
    return dividerIndex1 * dividerIndex2;
}

bool Day13::tryGetExpectedResultPart1(Result& outResult) const
{
    outResult = 5292;
    return true;
}

bool Day13::tryGetExpectedResultPart2(Result& outResult) const
{
    outResult = 23868;
    return true;
}
