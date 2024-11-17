/*
 * PerformanceTimer.h
 *
 * Provides utiliy to measure performance.
 */

#ifndef UTILIX_PERFORMANCE_TIMER
#define UTILIX_PERFORMANCE_TIMER

#include <chrono>
#include <string>

namespace UTLX
{
	class PerformanceTimer {
	public:
		PerformanceTimer(std::string id);
		~PerformanceTimer();
	private:
		const std::string m_id;
		std::chrono::high_resolution_clock::time_point m_start_time;
		std::chrono::high_resolution_clock::duration m_time_passed;

		void start();
		void stop();
		std::string str();
	};
}

#endif