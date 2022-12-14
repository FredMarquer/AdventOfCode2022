#pragma once

#include <string_view>

bool tryParse(const std::string_view& view, int32_t& outValue);
bool tryParse(const std::string_view& view, uint32_t& outValue);
bool tryParse(const std::string_view& view, int64_t& outValue);
bool tryParse(const std::string_view& view, uint64_t& outValue);

