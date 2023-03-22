#pragma once

#include <format>
#include <iostream>
#include <string>
#include <string_view>

#define info(fmt, ...) std::cout << "\033[38;2;230;230;230m" << std::vformat(fmt, std::make_format_args(__VA_ARGS__)) << "\033[0m" << '\n'

// Generate some conflict with the std...
//#define warning(fmt, ...) std::cout << "\033[38;2;230;180;0m" << std::vformat(fmt, std::make_format_args(__VA_ARGS__)) << "\033[0m" << '\n'

#define error(fmt, ...) std::cerr << "\033[38;2;230;0;0m" << std::vformat(fmt, std::make_format_args(__VA_ARGS__)) << "\033[0m" << '\n'

#ifdef NDEBUG
#define debug(fmt, ...)
#else
#define debug(fmt, ...) std::cout << "\033[38;2;150;150;150m" << std::vformat(fmt, std::make_format_args(__VA_ARGS__)) << "\033[0m" << '\n'
#endif // DEBUG

