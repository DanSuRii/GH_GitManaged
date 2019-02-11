#include "pch.h"
#include "IOCtx.h"

namespace NS_DPNET
{

	EIOTyp IOCtx::GetTyp() { return EIO_Invalid; }

	AcceptIO::AcceptIO()
	{
		if (false == ctx.isValid())
		{
			LOG_FN(", Invalid Socket");
			return;
		}
	}
}
