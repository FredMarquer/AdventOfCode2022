#include "Parsing.h"

#include <charconv>
#include <string_view>

#include "Log.h"

bool tryParse(const std::string_view& view, int32_t& outValue)
{
    auto result = std::from_chars(view.data(), view.data() + view.size(), outValue);
    if (result.ec != std::errc()) {
        error("can't parse string view: {}", view);
        return false;
    }

    return true;
}

bool tryParse(const std::string_view& view, uint32_t& outValue)
{
    auto result = std::from_chars(view.data(), view.data() + view.size(), outValue);
    if (result.ec != std::errc()) {
        error("can't parse string view: {}", view);
        return false;
    }

    return true;
}

bool tryParse(const std::string_view& view, int64_t& outValue)
{
    auto result = std::from_chars(view.data(), view.data() + view.size(), outValue);
    if (result.ec != std::errc()) {
        error("can't parse string view: {}", view);
        return false;
    }

    return true;
}

bool tryParse(const std::string_view& view, uint64_t& outValue)
{
    auto result = std::from_chars(view.data(), view.data() + view.size(), outValue);
    if (result.ec != std::errc()) {
        error("can't parse string view: {}", view);
        return false;
    }

    return true;
}

