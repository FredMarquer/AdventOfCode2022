#pragma once

#include <memory>

#include "Solver.h"

const int32_t DayCount = 18;

std::unique_ptr<Solver> createDay(int32_t dayNumber);

