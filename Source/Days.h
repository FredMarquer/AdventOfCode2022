#pragma once

#include <memory>

#include "Day.h"

const int32_t DayCount = 17;

std::unique_ptr<Day> createDay(int32_t dayNumber);

