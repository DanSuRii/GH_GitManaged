#pragma once

#include "IOCtx.h"

namespace NS_DPNET
{
	class ICompletionKey
	{
	public:
		virtual void HandleIO(IOCtx*, DWORD) = 0;
	};
}
