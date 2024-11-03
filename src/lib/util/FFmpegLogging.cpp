/*
 * FFmpegLogging.cpp
 *
 * Control the logging functionality provide by FFmpeg.
 */

#include "FFmpegLogging.h"

extern "C" { 
#include <libavutil/log.h>
}

Utilix::LogType convert(int ffmpegLogLevel) {
    static std::vector<std::pair<const int, const Utilix::LogType>> typeMap = {
        {AV_LOG_QUIET  , Utilix::LogType::NONE },
        {AV_LOG_PANIC  , Utilix::LogType::PANIC},
        {AV_LOG_FATAL  , Utilix::LogType::ERROR},
        {AV_LOG_ERROR  , Utilix::LogType::ERROR},
        {AV_LOG_WARNING, Utilix::LogType::WARN },
        {AV_LOG_INFO   , Utilix::LogType::INFO },
        {AV_LOG_VERBOSE, Utilix::LogType::DEBUG},
        {AV_LOG_DEBUG  , Utilix::LogType::DEBUG},
        {AV_LOG_TRACE  , Utilix::LogType::TRACE},
    };
    const auto pred = [&ffmpegLogLevel](const auto& pair) {
        return pair.first == ffmpegLogLevel;
        };

    auto found = std::ranges::find_if(typeMap, pred);
    if (found != typeMap.end()) {
        return found->second;
    }
    std::cerr << "Error: Unknown FFmpeg AV_LOG_TYPE=" << ffmpegLogLevel << std::endl;
    return Utilix::LogType::NONE;
}

const int convert(Utilix::LogType logType) {
    static std::vector<std::pair<const Utilix::LogType, const int>> typeMap = {
        {Utilix::LogType::NONE , AV_LOG_QUIET   },
        {Utilix::LogType::PANIC, AV_LOG_PANIC   },
        //{Utilix::LogType::ERROR, AV_LOG_FATAL   },
        {Utilix::LogType::ERROR, AV_LOG_ERROR   },
        {Utilix::LogType::WARN , AV_LOG_WARNING },
        {Utilix::LogType::INFO , AV_LOG_INFO    },
        //{Utilix::LogType::DEBUG , AV_LOG_VERBOSE},
        {Utilix::LogType::DEBUG, AV_LOG_DEBUG   },
        {Utilix::LogType::TRACE, AV_LOG_TRACE   },
    };
    const auto pred = [&logType](const auto& pair) {
        return pair.first == logType;
        };

    auto found = std::ranges::find_if(typeMap, pred);
    if (found != typeMap.end()) {
        return found->second;
    }
    std::cerr << "Error: Unknown Utilix::LogType=" << logType << std::endl;
    return AV_LOG_QUIET;
}

void FFmpegLogging::SetLevel(Utilix::LogType level)
{
    av_log_set_level(convert(level));
}

void FFmpegLogging::ConnectLogger()
{
    av_log_set_callback(FFmpegLogging::log_callback);
}

void FFmpegLogging::Test()
{
    av_log(nullptr, AV_LOG_QUIET  , "FFmpegLogging test message [QUIET]\n");
    av_log(nullptr, AV_LOG_PANIC  , "FFmpegLogging test message [PANIC]\n");
    av_log(nullptr, AV_LOG_FATAL  , "FFmpegLogging test message [FATAL]\n");
    av_log(nullptr, AV_LOG_ERROR  , "FFmpegLogging test message [ERROR]\n");
    av_log(nullptr, AV_LOG_WARNING, "FFmpegLogging test message [WARNING]\n");
    av_log(nullptr, AV_LOG_INFO   , "FFmpegLogging test message [INFO]\n");
    av_log(nullptr, AV_LOG_VERBOSE, "FFmpegLogging test message [VERBOSE]\n");
    av_log(nullptr, AV_LOG_DEBUG  , "FFmpegLogging test message [DEBUG]\n");
    av_log(nullptr, AV_LOG_TRACE  , "FFmpegLogging test message [TRACE]\n");
}

void FFmpegLogging::log_callback(void* ptr, int level, const char* fmt, va_list vl)
{
    static std::mutex log_mutex;
    std::lock_guard<std::mutex> lock(log_mutex);

    // Retrieve the AVClass to get class-specific information
    const AVClass* av_class = ptr ? *(const AVClass**)ptr : nullptr;
    const char* class_name = av_class ? av_class->class_name : "FFmpegPlusPlusLib";

    // Create a temporary buffer to hold the formatted message
    char message[1024];
    vsnprintf(message, sizeof(message), fmt, vl);
    
    LOG_FFMPEG(convert(level), message, class_name);
}
