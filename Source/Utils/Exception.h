#pragma once

#include <exception>
#include <format>
#include <string>
#include <string_view>

class Exception : public std::exception
{
private:
    std::string message;

public:
    Exception(std::string&& message) : message(message) {}

    const char* what() const noexcept override { return message.data(); }
};

#define exception(fmt, ...) throw Exception(std::vformat(fmt, std::make_format_args(__VA_ARGS__)))

