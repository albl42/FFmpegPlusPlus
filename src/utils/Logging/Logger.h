/*
 * BasicLogger
 *
 * Provides basic logging functionality.
 */

#ifndef UTILIX_BASIC_LOGGER
#define UTILIX_BASIC_LOGGER

#include <string>

namespace Utilix {
	class BasicLogger {
	public:
		BasicLogger();
		~BasicLogger();

		static void log_debug(std::string msg, std::string src_file, int line);
		static void log_info(std::string msg, std::string src_file, int line);
		static void log_warn(std::string warning, std::string src_file, int line);
		static void log_error(std::string error, std::string src_file, int line);
		static void log_time(std::string msg, std::string src_file, int line);

	private:
		static void print_to_terminal(const std::string& type, const std::string& msg, const std::string& src_file, int line);
		static std::string_view get_file_substring(const std::string& str, const std::string_view& sequence);
	};

}

#define LOG_DEBUG(msg) Utilix::BasicLogger::log_debug(msg, __FILE__, __LINE__);
#define LOG_INFO(msg) Utilix::BasicLogger::log_info(msg, __FILE__, __LINE__);
#define LOG_WARN(msg) Utilix::BasicLogger::log_warn(msg, __FILE__, __LINE__);
#define LOG_ERROR(msg) Utilix::BasicLogger::log_error(msg, __FILE__, __LINE__);
#define LOG_TIME(msg) Utilix::BasicLogger::log_time(msg, __FILE__, __LINE__);

#endif