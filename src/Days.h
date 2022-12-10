#pragma once

#include "Day.h"
#include "Day01.h"
#include "Day02.h"
#include "Day03.h"
#include "Day04.h"
#include "Day05.h"
#include "Day06.h"
#include "Day07.h"
#include "Day08.h"
#include "Day09.h"
#include "Day10.h"
#include "Log.h"

const int DayCount = 10;

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
	default:
		error("invalid day number: {}", dayNumber);
		break;
	}

	return std::unique_ptr<Day>(dayPtr);
}

