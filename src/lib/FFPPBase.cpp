/*
 * FFPPBase.cpp
 *
 * Wrapper for the base struct AVClass used by FFmpeg.
 */

#include "FFPPBase.h"

extern "C" {
#include <libavutil\avutil.h>
}

FFPP::FFPPBase::FFPPBase(AVClass* base)
{
	m_base.reset(base);
}

AVClass* FFPP::FFPPBase::get() const
{
	return m_base.get();
}
