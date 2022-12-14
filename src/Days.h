#pragma once

#include "Day.h"

const int DayCount = 12;

std::unique_ptr<Day> createDay(int dayNumber);

