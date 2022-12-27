#include "StdAfx.h"

#include "ScopeProfiler.h"

#ifdef ENABLE_PROFILING

#include <chrono>

#include "Log.h"

ScopeProfiler::ScopeProfiler(const char* name) : name(name)
{
	startTime = std::chrono::high_resolution_clock::now();
}

ScopeProfiler::~ScopeProfiler()
{
	auto endTime = std::chrono::high_resolution_clock::now();
	auto timeInMicroSeconds = (endTime - startTime) / std::chrono::microseconds(1);
	float timeInMilliSeconds = (float)timeInMicroSeconds * 0.001f;
	log("{} run in: {} ms", name, timeInMilliSeconds);
}

#endif // ENABLE_PROFILING