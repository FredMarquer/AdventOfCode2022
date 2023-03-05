#pragma once

#include <memory>

#include "Solver.h"

const int32_t DayCount = 24;

std::unique_ptr<Solver> createDay(int32_t dayNumber);

