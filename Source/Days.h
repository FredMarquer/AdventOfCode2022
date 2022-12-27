#pragma once

#include <memory>

#include "Day.h"

const int DayCount = 17;

std::unique_ptr<Day> createDay(int dayNumber);

