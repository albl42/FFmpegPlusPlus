/*
 * FFmpegPlusPlusLib
 *
 * Library providing FFmpeg functionality.
 */

#include "../utils/Logging/Logger.h"

int main()
{
    LOG_DEBUG("Library debug message!\n");
    LOG_INFO("Library info message!\n");
    LOG_WARN("Library warning message!\n");
    LOG_ERROR("Library error message!\n");
    LOG_TIME("Library time message!\n");
}