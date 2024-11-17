/*
 * FFmpegPlusPlusLib
 *
 * Library providing FFmpeg functionality.
 */

#include "../utils/Logging/Logger.h"
#include "util/FFmpegLogging.h"
#include "util/FFmpegParams.h"

extern "C" {
#include <libavcodec\avcodec.h>
}

int main()
{
    //ACTIVATE_TERMINAL_LOGGER_MASK(LOG_TYPE_INFO | LOG_TYPE_TIME);
    ACTIVATE_TERMINAL_LOGGER();
    ACTIVATE_DEFAULT_FILE_LOGGER();

    FFmpegLogging::SetLevel(Utilix::LogType::TRACE);
    FFmpegLogging::ConnectLogger();
    FFmpegLogging::Test();

    // Find the AAC audio encoder
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        LOG_ERROR("Codec not found\n");
        return 0;
    }

    // Allocate and initialize codec context for the encoder
    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        LOG_ERROR("Could not allocate audio codec context\n");
        return 0;
    }

    codec_ctx->time_base.num = 1;
    codec_ctx->time_base.den = 1000;
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    codec_ctx->width = 1920;
    codec_ctx->height = 1200;

    // Initialize the codec context with default settings
    if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
        LOG_ERROR("Could not open codec\n");
        avcodec_free_context(&codec_ctx);
        return 0;
    }

    FFPP::FFPPArgs params = FFPP::FFPPArgs((AVClass *)codec_ctx);

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