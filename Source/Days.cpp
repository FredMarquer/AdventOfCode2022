#include "Days.h"

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
#include "Utils/Log.h"

std::unique_ptr<Day> createDay(int dayNumber)
{
	Day* dayPtr = nullptr;
	switch (dayNumber)
	{
	case 1: dayPtr = new Day01(); break;
	case 2: dayPtr = new Day02(); break;
	case 3: dayPtr = new Day03(); break;
	case 4: dayPtr = new Day04(); break;
	case 5: dayPtr = new Day05(); break;
	case 6: dayPtr = new Day06(); break;
	case 7: dayPtr = new Day07(); break;
	case 8: dayPtr = new Day08(); break;
	case 9: dayPtr = new Day09(); break;
	case 10: dayPtr = new Day10(); break;
	case 11: dayPtr = new Day11(); break;
	case 12: dayPtr = new Day12(); break;
	case 13: dayPtr = new Day13(); break;
	default:
		error("invalid day number: {}", dayNumber);
		break;
	}

	return std::unique_ptr<Day>(dayPtr);
}

