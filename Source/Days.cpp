#include "Days.h"

#include <memory>

#include "Day.h"
#include "Days/Day01.h"
#include "Days/Day02.h"
#include "Days/Day03.h"
#include "Days/Day04.h"
#include "Days/Day05.h"
#include "Days/Day06.h"
#include "Days/Day07.h"
#include "Days/Day08.h"
#include "Days/Day09.h"
#include "Days/Day10.h"
#include "Days/Day11.h"
#include "Days/Day12.h"
#include "Days/Day13.h"
#include "Days/Day14.h"
#include "Days/Day15.h"
#include "Days/Day16.h"
#include "Days/Day17.h"
#include "Utils/Log.h"

std::unique_ptr<Day> createDay(int32_t dayNumber)
{
	std::unique_ptr<Day> day;
	switch (dayNumber) {
	case 1: day = std::make_unique<Day01>(); break;
	case 2: day = std::make_unique<Day02>(); break;
	case 3: day = std::make_unique<Day03>(); break;
	case 4: day = std::make_unique<Day04>(); break;
	case 5: day = std::make_unique<Day05>(); break;
	case 6: day = std::make_unique<Day06>(); break;
	case 7: day = std::make_unique<Day07>(); break;
	case 8: day = std::make_unique<Day08>(); break;
	case 9: day = std::make_unique<Day09>(); break;
	case 10: day = std::make_unique<Day10>(); break;
	case 11: day = std::make_unique<Day11>(); break;
	case 12: day = std::make_unique<Day12>(); break;
	case 13: day = std::make_unique<Day13>(); break;
	case 14: day = std::make_unique<Day14>(); break;
	case 15: day = std::make_unique<Day15>(); break;
	case 16: day = std::make_unique<Day16>(); break;
	case 17: day = std::make_unique<Day17>(); break;
	default:
		error("invalid day number: {}", dayNumber);
		day = nullptr;
		break;
	}

	return day;
}

