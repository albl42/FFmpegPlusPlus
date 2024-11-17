/*
 * PerformanceTimer.cpp
 *
 * Provides utiliy to measure performance.
 */
#include "PerformanceTimer.h"

using namespace UTLX;
using namespace std::chrono;

UTLX::PerformanceTimer::PerformanceTimer(std::string id)
	: m_id(id)
{
	start();
}

UTLX::PerformanceTimer::~PerformanceTimer()
{
}

void UTLX::PerformanceTimer::start()
{
	m_start_time = high_resolution_clock::now();
}

void UTLX::PerformanceTimer::stop()
{
	auto end_time = high_resolution_clock::now();
	m_time_passed = duration_cast<nanoseconds>(end_time - m_start_time);
}

std::string UTLX::PerformanceTimer::str()
{
	nanoseconds nanosec = floor<nanoseconds>(m_time_passed);
	microseconds microsec = floor<microseconds>(m_time_passed);
	milliseconds milisec = floor<milliseconds>(m_time_passed);
	seconds sec = floor<seconds>(m_time_passed);
	minutes min = floor<minutes>(m_time_passed);
	hours h = floor<hours>(m_time_passed);
	return std::format("{} took: ", m_time_passed);
}
