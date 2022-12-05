#pragma once

#include <iostream>

#include "Day.h"
#include "Day01.h"
#include "Day02.h"
#include "Day03.h"
#include "Day04.h"
#include "Day05.h"

const int DayCount = 5;

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
	default:
		std::cout << "invalid day number: " << dayNumber << std::endl;
		break;
	}

	return std::unique_ptr<Day>(dayPtr);
}
