/*
 * FFmpegPlusPlusApp
 *
 * Executable application that can be executed.
 */

#include "../utils/Logging/Logger.h"

int main()
{
    LOG_DEBUG("Application debug message!\n");
    LOG_INFO("Application info message!\n");
    LOG_WARN("Application warning message!\n");
    LOG_ERROR("Application error message!\n");
    LOG_TIME("Application time message!\n");
}