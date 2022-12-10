#pragma once

#include <format>
#include <iostream>
#include <string>
#include <string_view>

#define log(fmt, ...) std::cout << "\033[38;2;230;230;230m" << std::vformat(fmt, std::make_format_args(__VA_ARGS__)) << "\033[0m" << std::endl

#define warning(fmt, ...) std::cout << "\033[38;2;230;180;0m" << std::vformat(fmt, std::make_format_args(__VA_ARGS__)) << "\033[0m" << std::endl

#define error(fmt, ...) std::cout << "\033[38;2;230;0;0m" << std::vformat(fmt, std::make_format_args(__VA_ARGS__)) << "\033[0m" << std::endl

#ifdef NDEBUG
	#define debug(fmt, ...)
#else
	#define debug(fmt, ...) std::cout << "\033[38;2;150;150;150m" << std::vformat(fmt, std::make_format_args(__VA_ARGS__)) << "\033[0m" << std::endl
#endif // DEBUG
