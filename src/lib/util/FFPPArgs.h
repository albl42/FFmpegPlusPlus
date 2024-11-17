/*
 * FFmpegParams.h
 *
 * Wrapper for parameters used by FFmpeg (AVOptions and AVDictionary).
 */

#ifndef FFMPEG_PLUS_PLUS_PARAMS
#define FFMPEG_PLUS_PLUS_PARAMS

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "../FFPPBase.h"

namespace FFPP {
	struct AVClass;

	class FFPPArg {
	public:
		std::string name = "";
		std::string help = "";
		std::optional<int> offset = std::nullopt;
		std::string type = "";
		std::string ctype = "";
		//Note: const AVOptionArrayDef* missing
		//Note: AVRational missing
		std::variant<int64_t, double, std::string> default_val{};
		std::optional<double> min = std::nullopt;
		std::optional<double> max = std::nullopt;
		std::optional<int> flags = std::nullopt;
		std::string unit = "";

		std::string str();
	};

	class FFPPArgs {
	public:
		explicit FFPPArgs(FFPPBase& base);
		~FFPPArgs() = default;

	private:
		std::vector<FFPPArg> m_params;
		void initialize();

		AVClass* m_obj = nullptr;
	};
}
#endif