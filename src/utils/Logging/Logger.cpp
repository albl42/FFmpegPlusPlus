/*
 * BasicLogger
 *
 * Provides basic logging functionality.
 */

#include "Logger.h"

#include <iostream>
#include <format>
#include <ranges>

using namespace Utilix;

constexpr std::string_view SolutionRootFolder = "FFmpegPlusPlus";

BasicLogger::BasicLogger()
{
}

BasicLogger::~BasicLogger()
{
}

void BasicLogger::log_debug(std::string msg, std::string src_file, int line)
{
	print_to_terminal("DEBUG", msg, src_file, line);
}

void BasicLogger::log_info(std::string msg, std::string src_file, int line)
{
	print_to_terminal("INFO ", msg, src_file, line);
}

void BasicLogger::log_warn(std::string warning, std::string src_file, int line)
{
	print_to_terminal("WARN ", warning, src_file, line);
}

void BasicLogger::log_error(std::string error, std::string src_file, int line)
{
	print_to_terminal("ERROR", error, src_file, line);
}

void BasicLogger::log_time(std::string msg, std::string src_file, int line)
{
	print_to_terminal("TIME ", msg, src_file, line);
}

void BasicLogger::print_to_terminal(const std::string& type, const std::string& msg, const std::string& src_file, int line)
{
	// Note: printf has better performance than cout
	printf("%s", std::format("[{}] {} @ {}: {}", type, get_file_substring(src_file, SolutionRootFolder), line, msg).c_str());
}

std::string_view BasicLogger::get_file_substring(const std::string& msg, const std::string_view& root)
{
	auto found = std::ranges::search(msg, root);
	if (found.begin() != msg.end()) {
		auto result_view = std::string_view(found.begin(), msg.end());
		return { result_view.begin(), result_view.end() };
	}

	return msg;
}