#pragma once

#include <string_view>

void parse(const std::string_view& view, int32_t& outValue);
void parse(const std::string_view& view, uint32_t& outValue);
void parse(const std::string_view& view, int64_t& outValue);
void parse(const std::string_view& view, uint64_t& outValue);

