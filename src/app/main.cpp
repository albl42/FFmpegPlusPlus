/*
 * FFmpegPlusPlusApp
 *
 * Executable application.
 */

#include "../utils/Logging/Logger.h"

int main()
{
    ACTIVATE_TERMINAL_LOGGER();
    ACTIVATE_TERMINAL_LOGGER();

    LOG_TRACE("Library trace message is here!\n");
    LOG_TIME("Library time messageis here!\n");
    LOG_DEBUG("Library debug message is here!\n");
    LOG_INFO("Library info message is here!\n");
    LOG_WARN("Library warning message is here!\n");
    LOG_ERROR("Library error message is here!\n");
    LOG_PANIC("Library panik message is here!\n");
}