#pragma once

#include <chrono>
#include <string>

class ScopeProfiler
{
private:
	std::string name;
	std::chrono::steady_clock::time_point startTime;

public:
	ScopeProfiler(const std::string& name);
	~ScopeProfiler();
};

#ifdef ENABLE_PROFILING
	#define profileScope(name) ScopeProfiler scopeProfiler(name);
#else
	#define profileScope(name)
#endif // DEBUG

