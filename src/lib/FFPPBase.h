/*
 * FFPPBase.h
 *
 * Wrapper for the base struct AVClass used by FFmpeg.
 */

#ifndef FFMPEG_PLUS_PLUS_BASE
#define FFMPEG_PLUS_PLUS_BASE

#include <memory>

struct AVClass;

namespace FFPP
{
	/// <summary>
	/// FFPPBase is a wrapper for the AVClass struct used by FFmpeg.
	/// </summary>
	class FFPPBase {
	public:
		FFPPBase(AVClass* base);
		~FFPPBase() = default;

		AVClass* get() const;
	private:
		std::unique_ptr<AVClass> m_base = nullptr;
	};
}

#endif