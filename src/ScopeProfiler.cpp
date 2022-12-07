#include "ScopeProfiler.h"

#include <chrono>
#include <iostream>

ScopeProfiler::ScopeProfiler(const std::string& name) : name(name)
{
	startTime = std::chrono::high_resolution_clock::now();
}

ScopeProfiler::~ScopeProfiler()
{
	auto endTime = std::chrono::high_resolution_clock::now();
	auto timeInMicroSeconds = (endTime - startTime) / std::chrono::microseconds(1);
	float timeInMilliSeconds = (float)timeInMicroSeconds * 0.001f;
	std::cout << name << " run in : " << timeInMilliSeconds << " ms" << std::endl;
}
