#pragma once

#ifdef ENABLE_PROFILING

#include <chrono>

class ScopeProfiler
{
private:
	const char* name;
	std::chrono::steady_clock::time_point startTime;
	uint64_t startAllocationCount;

public:
	ScopeProfiler(const char* name);
	~ScopeProfiler();
};

#define profileScope(name) ScopeProfiler scopeProfiler(name);

#else
#define profileScope(name)
#endif // ENABLE_PROFILING

