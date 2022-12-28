#include "StdAfx.h"

#include "Parsing.h"

#include <charconv>

#include "Exception.h"

void parse(std::string_view view, int32_t& outValue)
{
    auto result = std::from_chars(view.data(), view.data() + view.size(), outValue);
    if (result.ec != std::errc())
        exception("can't parse string view: {}", view);
}

void parse(std::string_view view, uint32_t& outValue)
{
    auto result = std::from_chars(view.data(), view.data() + view.size(), outValue);
    if (result.ec != std::errc())
        exception("can't parse string view: {}", view);
}

void parse(std::string_view view, int64_t& outValue)
{
    auto result = std::from_chars(view.data(), view.data() + view.size(), outValue);
    if (result.ec != std::errc())
        exception("can't parse string view: {}", view);
}

void parse(std::string_view view, uint64_t& outValue)
{
    auto result = std::from_chars(view.data(), view.data() + view.size(), outValue);
    if (result.ec != std::errc())
        exception("can't parse string view: {}", view);
}

