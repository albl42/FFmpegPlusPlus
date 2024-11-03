/*
 * FFmpegLogging.h
 *
 * Control the logging functionality provide by FFmpeg.
 */

#ifndef FFMPEG_PLUS_PLUS_LOGGING
#define FFMPEG_PLUS_PLUS_LOGGING

#include <mutex>

#include "../../utils/Logging/Logger.h"

class FFmpegLogging {
public:
	static void SetLevel(Utilix::LogType level);
	static void ConnectLogger();

	static void Test();

private:
	static void log_callback(void* ptr, int level, const char* fmt, va_list vl);
};

#endif