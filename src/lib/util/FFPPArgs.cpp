/*
 * FFmpegLogging.cpp
 *
 * Control the logging functionality provide by FFmpeg.
 */

// Todo: Add support for default values

#include "FFPPArgs.h"

#include <numeric>
#include <string>
#include <sstream>
#include <map>

#include "../../utils/Logging/Logger.h"

extern "C" {
#include <libavutil\avutil.h>
#include <libavutil\opt.h>
#include <libavutil\dict.h>
}

using namespace FFPP;

std::string
av_opt_type_name(const AVOptionType& type) {
	switch(type){
		case AVOptionType::AV_OPT_TYPE_FLAGS     : return "flags";
		case AVOptionType::AV_OPT_TYPE_INT       : return "int";
		case AVOptionType::AV_OPT_TYPE_INT64     : return "int64";
		case AVOptionType::AV_OPT_TYPE_DOUBLE    : return "doouble";
		case AVOptionType::AV_OPT_TYPE_FLOAT     : return "float";
		case AVOptionType::AV_OPT_TYPE_STRING    : return "string";
		case AVOptionType::AV_OPT_TYPE_RATIONAL  : return "rational";
		case AVOptionType::AV_OPT_TYPE_BINARY    : return "binary";
		case AVOptionType::AV_OPT_TYPE_DICT      : return "dict";
		case AVOptionType::AV_OPT_TYPE_UINT64    : return "unit64";
		case AVOptionType::AV_OPT_TYPE_CONST     : return "const";
		case AVOptionType::AV_OPT_TYPE_IMAGE_SIZE: return "image_size";
		case AVOptionType::AV_OPT_TYPE_PIXEL_FMT : return "pixel_fmt";
		case AVOptionType::AV_OPT_TYPE_SAMPLE_FMT: return "sample_fmt";
		case AVOptionType::AV_OPT_TYPE_VIDEO_RATE: return "video_rate";
		case AVOptionType::AV_OPT_TYPE_DURATION  : return "duration";
		case AVOptionType::AV_OPT_TYPE_COLOR     : return "color";
		case AVOptionType::AV_OPT_TYPE_BOOL      : return "bool";
		case AVOptionType::AV_OPT_TYPE_CHLAYOUT  : return "chlayout";
		case AVOptionType::AV_OPT_TYPE_UINT      : return "uint";
		case AVOptionType::AV_OPT_TYPE_FLAG_ARRAY: return "array";
		default: {
			LOG_ERROR("Unsupported AVOptionType!\n");
			return "<ERROR>";
		}
	};
}

std::string
av_opt_type_ctype(const AVOptionType& type) {
	switch (type) {
		case AVOptionType::AV_OPT_TYPE_FLAGS     : return "unsigned int";
		case AVOptionType::AV_OPT_TYPE_INT       : return "int";
		case AVOptionType::AV_OPT_TYPE_INT64     : return "int64_t";
		case AVOptionType::AV_OPT_TYPE_DOUBLE    : return "doouble";
		case AVOptionType::AV_OPT_TYPE_FLOAT     : return "float";
		case AVOptionType::AV_OPT_TYPE_STRING    : return "uint8_t*";
		case AVOptionType::AV_OPT_TYPE_RATIONAL  : return "AVRational";
		case AVOptionType::AV_OPT_TYPE_BINARY    : return "uint8_t*";
		case AVOptionType::AV_OPT_TYPE_DICT      : return "AVDictionary*";
		case AVOptionType::AV_OPT_TYPE_UINT64    : return "uint64_t";
		case AVOptionType::AV_OPT_TYPE_CONST     : return "NULL";
		case AVOptionType::AV_OPT_TYPE_IMAGE_SIZE: return "[int, int]";
		case AVOptionType::AV_OPT_TYPE_PIXEL_FMT : return "AVPixelFormat";
		case AVOptionType::AV_OPT_TYPE_SAMPLE_FMT: return "AVSampleFormat";
		case AVOptionType::AV_OPT_TYPE_VIDEO_RATE: return "AVRational";
		case AVOptionType::AV_OPT_TYPE_DURATION  : return "int64_t";
		case AVOptionType::AV_OPT_TYPE_COLOR     : return "[uint8_t, uint8_t, uint8_t, uint8_t]";
		case AVOptionType::AV_OPT_TYPE_BOOL      : return "int";
		case AVOptionType::AV_OPT_TYPE_CHLAYOUT  : return "AVChannelLayout";
		case AVOptionType::AV_OPT_TYPE_UINT      : return "unsigned int";
		case AVOptionType::AV_OPT_TYPE_FLAG_ARRAY: return "[...]";
		default: {
			LOG_ERROR("Unsupported AVOptionType!\n");
			return "<ERROR>";
		}
	};
}

FFPPArgs::FFPPArgs(FFPPBase& base)
	: m_obj(base.get())
{
	if (m_obj) {
		initialize();
	}
}

void FFPPArgs::initialize()
{
	const AVOption* option = nullptr;

	LOG_DEBUG("Listing all options for AVCodecContext:\n");
	while ((option = av_opt_next(m_obj, option)) != nullptr) {
		
		FFPPArg new_param;

		if (option->name) {
			new_param.name = option->name;
		}
		
		if (option->help) {
			new_param.help = option->help;
		}

		new_param.offset = option->offset;
		
		if (option->help) {
			new_param.help = option->help;
		}

		new_param.type = av_opt_type_name(option->type);
		new_param.ctype = av_opt_type_ctype(option->type);

		/*if (option->default_val) {
			new_param.default_val = option->default_val;
		}*/
		
		new_param.min = option->min;
		new_param.max = option->max;
		new_param.flags = option->flags;

		if (option->unit) {
			new_param.unit = option->unit;
		}

		if (option->flags & AV_OPT_FLAG_READONLY) {
			new_param.name = new_param.name.append(" (Read-only)");
		}
		if (option->flags & AV_OPT_FLAG_DEPRECATED) {
			new_param.name = new_param.name.append(" (Read-only)");
		}

		auto msg = new_param.str();
		LOG_INFO(msg);
		m_params.emplace_back(new_param);
	}
}

std::string FFPPArg::str()
{
	std::stringstream s("");
	s << "\n  AVOption :";
	s << "\n    name       : " << name;
	s << "\n    help       : " << help;
	s << "\n    offset     : " << offset.value();
	s << "\n    type       : " << type;
	s << "\n    ctype      : " << ctype;
	//s << "\n    default_val: " << std::get_if<int64_t>(default_val);
	s << "\n    min        : " << min.value();
	s << "\n    max        : " << max.value();
	s << "\n    flags      : " << flags.value();
	s << "\n    unit       : " << unit;
	s << "\n";
	return s.str();
}
