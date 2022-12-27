#include "StdAfx.h"

#include "ScopeProfiler.h"

#ifdef ENABLE_PROFILING

#include <atomic>
#include <chrono>

#include "Log.h"

static std::atomic_uint64_t totalAllocationCount = 0;

void* operator new(size_t size)
{
	++totalAllocationCount;
	
    void* ptr = malloc(size);

    if (!ptr)
        throw std::bad_alloc();

	return ptr;
}

ScopeProfiler::ScopeProfiler(const char* name) : name(name)
{
	startTime = std::chrono::high_resolution_clock::now();
	startAllocationCount = totalAllocationCount;
}

ScopeProfiler::~ScopeProfiler()
{
	uint64_t allocationCount = totalAllocationCount - startAllocationCount;

	auto endTime = std::chrono::high_resolution_clock::now();
	auto timeInMicroSeconds = (endTime - startTime) / std::chrono::microseconds(1);
	float timeInMilliSeconds = (float)timeInMicroSeconds * 0.001f;

	log("{} run in: {} ms", name, timeInMilliSeconds);
	log("{} allocation count: {}", name, allocationCount);
}

#endif // ENABLE_PROFILING