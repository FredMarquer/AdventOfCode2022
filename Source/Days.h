#pragma once

#include <memory>

#include "Solver.h"

const int32_t DayCount = 17;

std::unique_ptr<Solver> createDay(int32_t dayNumber);

