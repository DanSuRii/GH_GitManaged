#include "pch.h"
#include "IOCtx.h"

namespace NS_DPNET
{
	IOCtx::IOCtx()
	{
		if (false == ctx.isValid())
		{
			LOG_FN(", Invalid Socket");
			return;
		}
	}
	EIOTyp IOCtx::GetTyp() { return EIO_Invalid; }

	AcceptIO::AcceptIO()
	{
	}
}
