#pragma once

#include <memory>

#include "Solver.h"

const int32_t DayCount = 20;

std::unique_ptr<Solver> createDay(int32_t dayNumber);

