/*
 * Logger.h
 *
 * Provides logging functionality.
 */

#ifndef UTILIX_BASIC_LOGGER
#define UTILIX_BASIC_LOGGER

#include <string>
#include <iostream>
#include <syncstream>
#include <fstream>
#include <vector>

namespace UTLX {
	/**
	* Enumeration of logging types.
	*/
	const enum LogType {
		NONE   = 0b0000000,
		TRACE  = 0b0000001,
		TIME   = 0b0000010,
		DEBUG  = 0b0000100,
		INFO   = 0b0001000,
		WARN   = 0b0010000,
		ERROR  = 0b0100000,
		PANIC  = 0b1000000,
		ALL    = 0b1111111, // internal usage
	};

	constexpr inline LogType operator~ (LogType a) { return static_cast<LogType>(~static_cast<std::underlying_type<LogType>::type>(a)); }
	constexpr inline LogType operator| (LogType a, LogType b) { return static_cast<LogType>(static_cast<std::underlying_type<LogType>::type>(a) | static_cast<std::underlying_type<LogType>::type>(b)); }
	constexpr inline LogType operator& (LogType a, LogType b) { return static_cast<LogType>(static_cast<std::underlying_type<LogType>::type>(a) & static_cast<std::underlying_type<LogType>::type>(b)); }
	constexpr inline LogType operator^ (LogType a, LogType b) { return static_cast<LogType>(static_cast<std::underlying_type<LogType>::type>(a) ^ static_cast<std::underlying_type<LogType>::type>(b)); }
	const inline LogType& operator|= (LogType& a, LogType b) { return reinterpret_cast<LogType&>(reinterpret_cast<std::underlying_type<LogType>::type&>(a) |= static_cast<std::underlying_type<LogType>::type>(b)); }
	const inline LogType& operator&= (LogType& a, LogType b) { return reinterpret_cast<LogType&>(reinterpret_cast<std::underlying_type<LogType>::type&>(a) &= static_cast<std::underlying_type<LogType>::type>(b)); }
	const inline LogType& operator^= (LogType& a, LogType b) { return reinterpret_cast<LogType&>(reinterpret_cast<std::underlying_type<LogType>::type&>(a) ^= static_cast<std::underlying_type<LogType>::type>(b)); }

	/**
	* Abstract logger base class.
	*/
	class Logger {
	public:
		virtual ~Logger() = default;

		void log(const std::string_view& msg) const;

		bool cmp_type(const LogType& type) const;

	protected:
		Logger() = default;

		void set_ostream(std::ostream& ostream);

		void set_type_mask(const LogType& mask);

		std::string m_logger_name;
	private:
		std::ostream* m_out = nullptr;
		LogType m_type_mask = LogType::NONE;
	};

	/**
	* Basic terminal logger class.
	*/
	class TerminalLogger : public Logger {
	public:
		TerminalLogger(LogType typemask);

		~TerminalLogger() override = default;
	};

	/**
	* Basic file logger class.
	*/
	class FileLogger : public Logger {
	public:
		FileLogger(std::string filepath, LogType typemask);

		~FileLogger() override;

	private:
		std::ofstream m_file_out;
	};

	/**
	* Auxillary class providing consistent formatting for log messages.
	*/
	class LogFormatter {
	public:
		static std::string Format(
			const std::string_view& current_time,
			const std::string_view& log_type,
			const std::string_view& msg,
			const std::string_view& src,
			int line);

		static std::string Format(
			const std::string_view& current_time,
			const std::string_view& log_type,
			const std::string_view& msg,
			const std::string_view& src);
	};

	/**
	* Singleton logger pool responsible for creation and deletion of loggers.
	*/
	class LoggerPool {
	public:
		static LoggerPool& get_instance(); // Singleton

		~LoggerPool() = default;

		void add_terminal_logger(LogType typemask = LogType::ALL);

		void add_file_logger(std::string logger_path, LogType typemask = LogType::ALL);

		void add_file_logger(LogType typemask = LogType::ALL);

		void log(
			LogType type,
			const std::string& msg,
			const std::string& src_file,
			int line) const;

		void log(
			LogType type,
			const std::string& msg,
			const std::string& src_file) const;

	private:
		LoggerPool() = default; // Singleton

		std::string_view get_type(const LogType& type) const;

		std::string get_substring_after(
			const std::string& str,
			const std::string_view& sequence) const;

		std::string get_substring_until(
			const std::string& str,
			const std::string_view& sequence) const;

		std::vector<std::unique_ptr<Logger>> m_logger_pool;
	};
}

#define LOG_TYPE_TRACE Utilix::LogType::TRACE
#define LOG_TYPE_TIME  Utilix::LogType::TIME 
#define LOG_TYPE_DEBUG Utilix::LogType::DEBUG
#define LOG_TYPE_INFO  Utilix::LogType::INFO 
#define LOG_TYPE_WARN  Utilix::LogType::WARN 
#define LOG_TYPE_ERROR Utilix::LogType::ERROR
#define LOG_TYPE_PANIC Utilix::LogType::PANIC

#define ACTIVATE_TERMINAL_LOGGER()     Utilix::LoggerPool::get_instance().add_terminal_logger();
#define ACTIVATE_FILE_LOGGER(filepath) Utilix::LoggerPool::get_instance().add_file_logger((filepath));
#define ACTIVATE_DEFAULT_FILE_LOGGER() Utilix::LoggerPool::get_instance().add_file_logger();

#define ACTIVATE_TERMINAL_LOGGER_MASK(typemask)       Utilix::LoggerPool::get_instance().add_terminal_logger((typemask));
#define ACTIVATE_FILE_LOGGER_MASK(filepath, typemask) Utilix::LoggerPool::get_instance().add_file_logger((filepath), (typemask));
#define ACTIVATE_DEFAULT_FILE_LOGGER_MASK(typemask)   Utilix::LoggerPool::get_instance().add_file_logger((typemask));

#define LOG_TRACE(msg) Utilix::LoggerPool::get_instance().log(LOG_TYPE_TRACE, (msg), __FILE__, __LINE__);
#define LOG_TIME(msg)  Utilix::LoggerPool::get_instance().log(LOG_TYPE_TIME , (msg), __FILE__, __LINE__);
#define LOG_DEBUG(msg) Utilix::LoggerPool::get_instance().log(LOG_TYPE_DEBUG, (msg), __FILE__, __LINE__);
#define LOG_INFO(msg)  Utilix::LoggerPool::get_instance().log(LOG_TYPE_INFO , (msg), __FILE__, __LINE__);
#define LOG_WARN(msg)  Utilix::LoggerPool::get_instance().log(LOG_TYPE_WARN , (msg), __FILE__, __LINE__);
#define LOG_ERROR(msg) Utilix::LoggerPool::get_instance().log(LOG_TYPE_ERROR, (msg), __FILE__, __LINE__);
#define LOG_PANIC(msg) Utilix::LoggerPool::get_instance().log(LOG_TYPE_PANIC, (msg), __FILE__, __LINE__);

#define LOG_FFMPEG(type, msg, src) Utilix::LoggerPool::get_instance().log((type), (msg), (src));

#endif