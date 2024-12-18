/*
 * Logger.cpp
 *
 * Provides logging functionality.
 */

#include "Logger.h"

#include <format>
#include <ranges>
#include <array>
#include <chrono>
#include <filesystem>

#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak();
#else
#define DEBUG_BREAK std::cout << "Error: Debug break not supported on the current system." << std::endl;
#endif

using namespace UTLX;

// Todo remove dependency to hardcoded solution name
constexpr std::string_view SolutionRootFolder = "FFmpegPlusPlus";
constexpr std::string_view TerminalName = "Terminal";

namespace {

	/**
	* @brief TimeFormatter provides static methods to format time strings.
	*/
	class TimeFormatter {
	public:

		/**
		* @brief Returns the current time in the format: "YYYY-MM-DD HH:MM:SS.MMMMMMM"
		*/
		static std::string to_log_str() {
			const auto timePair = update_current_time();
			return std::format("{}-{:02}-{:02} {:02}:{:02}:{:02}.{:07}",
				timePair.first.year(),
				static_cast<unsigned>(timePair.first.month()),
				static_cast<unsigned>(timePair.first.day()),
				timePair.second.hours().count(),
				timePair.second.minutes().count(),
				timePair.second.seconds().count(),
				timePair.second.subseconds().count()
			);
		};

		/**
		* @brief Returns the current time in the format: "YYYYMMDD_HHMMSS_MMMMMMM"
		*/
		static std::string to_filename() {
			const auto timePair = update_current_time();
			return std::format("{}{:02}{:02}_{:02}{:02}{:02}_{:07}",
				timePair.first.year(),
				static_cast<unsigned>(timePair.first.month()),
				static_cast<unsigned>(timePair.first.day()),
				timePair.second.hours().count(),
				timePair.second.minutes().count(),
				timePair.second.seconds().count(),
				timePair.second.subseconds().count()
			);
		};

	private:
		typedef std::pair<
			std::chrono::year_month_day,
			std::chrono::hh_mm_ss<std::chrono::system_clock::duration>>
			TimePair;

		// Source: https://stackoverflow.com/questions/65646395/c-retrieving-current-date-and-time-fast
		static TimePair update_current_time() {
			auto tp = std::chrono::system_clock::now();
			static auto const tz = std::chrono::current_zone();
			static auto info = tz->get_info(tp);
			if (tp >= info.end) {
				info = tz->get_info(tp);
			}
			auto tpl = std::chrono::local_days{} + (tp + info.offset - std::chrono::sys_days{});
			auto tpd = floor<std::chrono::days>(tpl);
			return { std::chrono::year_month_day(tpd), std::chrono::hh_mm_ss(tpl - tpd) };
		};
	};
}

void
UTLX::Logger::log(const std::string_view& msg) const
{
	if (m_out) {
		std::osyncstream(*m_out) << msg;
	}
}

bool
UTLX::Logger::cmp_type(const LogType& type) const
{
	return static_cast<unsigned>(m_type_mask) & static_cast<unsigned>(type);
}

void
UTLX::Logger::set_ostream(std::ostream& ostream)
{
	m_out = &ostream;
}

void UTLX::Logger::set_type_mask(const LogType& type)
{
	m_type_mask = type;
}

UTLX::TerminalLogger::TerminalLogger(LogType typemask)
{
	set_ostream(std::cout);
	set_type_mask(typemask);
	m_logger_name = TerminalName;
}

UTLX::FileLogger::FileLogger(std::string filepath, LogType typemask)
{
	m_file_out.open(filepath, std::fstream::out | std::fstream::app);
	if (!m_file_out.good() || !m_file_out.is_open()) {
		std::cerr << "Error: Failed to create log file: " << filepath << std::endl;
		return;
	}
	std::cerr << "Created log file: " << filepath << std::endl;
	set_ostream(m_file_out);
	set_type_mask(typemask);
	m_logger_name = "File:" + filepath;
}

UTLX::FileLogger::~FileLogger()
{
	if (m_file_out.is_open()) {
		m_file_out.close();
	}
}

std::string
UTLX::LogFormatter::Format(
	const std::string_view& current_time,
	const std::string_view& log_type,
	const std::string_view& msg,
	const std::string_view& src,
	int line)
{
	return std::format("{} [{}] {}:{} {}", current_time, log_type, src, line, msg);
}

std::string
UTLX::LogFormatter::Format(
	const std::string_view& current_time,
	const std::string_view& log_type,
	const std::string_view& msg,
	const std::string_view& src)
{
	return std::format("{} [{}] {}: {}", current_time, log_type, src, msg);
}

LoggerPool&
UTLX::LoggerPool::get_instance()
{
	static LoggerPool instance;
	return instance;
}

void
UTLX::LoggerPool::add_terminal_logger(LogType typemask)
{
	m_logger_pool.emplace_back(std::make_unique<UTLX::TerminalLogger>(typemask));
}

void
UTLX::LoggerPool::add_file_logger(std::string logger_path, LogType typemask)
{
	m_logger_pool.emplace_back(std::make_unique<UTLX::FileLogger>(logger_path, typemask));
}

void
UTLX::LoggerPool::add_file_logger(LogType typemask)
{
	const std::string filename = TimeFormatter::to_filename() + ".log";
	const std::string currentpath = std::filesystem::current_path().string();
	const std::string rootpath = get_substring_until(currentpath, SolutionRootFolder);
	if (rootpath.empty()) {
		std::cerr << "Error: Failed to gather solution root path '" << SolutionRootFolder << "' from " << currentpath;
		add_file_logger(filename, typemask);
	}
	else {
		add_file_logger(rootpath + "\\logs\\" + filename, typemask);
	}
}

void
UTLX::LoggerPool::log(
	LogType type,
	const std::string& msg,
	const std::string& src,
	int line) const
{
	const std::string file_str = get_substring_after(src, SolutionRootFolder);
	const std::string_view type_str = get_type(type);
	const std::string time_str = TimeFormatter::to_log_str();

	const auto pred = [&type](const std::unique_ptr<Logger>& logger) -> bool
		{ return logger->cmp_type(type); };

	for (const auto& logger : m_logger_pool | std::views::filter(pred))
	{
		logger->log(LogFormatter::Format(time_str, type_str, msg, file_str, line));
	}
}

void
UTLX::LoggerPool::log(
	LogType type,
	const std::string& msg,
	const std::string& src) const
{
	const std::string_view type_str = get_type(type);
	const std::string time_str = TimeFormatter::to_log_str();

	const auto pred = [&type](const std::unique_ptr<Logger>& logger) -> bool
		{ return logger->cmp_type(type); };

	for (const auto& logger : m_logger_pool | std::views::filter(pred))
	{
		logger->log(LogFormatter::Format(time_str, type_str, msg, src));
	}
}

std::string_view
UTLX::LoggerPool::get_type(const LogType& type) const
{
	static std::vector<std::pair<LogType, const std::string>> typeMap = {
		{LogType::NONE,  "NONE "},
		{LogType::TRACE, "TRACE"},
		{LogType::TIME,  "TIME "},
		{LogType::DEBUG, "DEBUG"},
		{LogType::INFO,  "INFO "},
		{LogType::WARN,  "WARN "},
		{LogType::ERROR, "ERROR"},
		{LogType::PANIC, "PANIC"},
	};

	const auto pred = [&type](const auto& pair) {
		return pair.first == type;
		};

	auto found = std::ranges::find_if(typeMap, pred);
	if (found != typeMap.end()) {
		return found->second;
	}
	return "<invalid LogType>";
}

std::string
UTLX::LoggerPool::get_substring_after(
	const std::string& str,
	const std::string_view& sequence) const
{
	const auto found = str.rfind(sequence);
	if (found != std::string::npos) {
		return str.substr(found + sequence.length() + 1, str.length());
	}
	return "";
}

std::string
UTLX::LoggerPool::get_substring_until(
	const std::string& str,
	const std::string_view& sequence) const
{
	const auto found = str.rfind(sequence);
	if (found != std::string::npos) {
		return str.substr(0, found + sequence.length());
	}
	return "";
}
