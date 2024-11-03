/*
 * FFmpegPlusPlusLib
 *
 * Library providing FFmpeg functionality.
 */

#include "../utils/Logging/Logger.h"
#include "util/FFmpegLogging.h"

int main()
{
    ACTIVATE_TERMINAL_LOGGER_MASK(LOG_TYPE_INFO | LOG_TYPE_TIME);
    ACTIVATE_TERMINAL_LOGGER();
    ACTIVATE_DEFAULT_FILE_LOGGER();

    FFmpegLogging::SetLevel(Utilix::LogType::TRACE);
    FFmpegLogging::ConnectLogger();
    FFmpegLogging::Test();

    for (int i = 0; i < 1; i++) {
        LOG_TRACE("Library trace message is here!\n");
        LOG_TIME("Library time messageis here!\n");
        LOG_DEBUG("Library debug message is here!\n");
        LOG_INFO("Library info message is here!\n");
        LOG_WARN("Library warning message is here!\n");
        LOG_ERROR("Library error message is here!\n");
        LOG_PANIC("Library panik message is here!\n");
    }
};