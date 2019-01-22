#include "pch.h"

class Sheet
{
};

using PSheet = std::shared_ptr<Sheet>;

template< class... Args >
PSheet NewSheet(Args&& ...args)
{
	return std::make_shared<Sheet>(args...);
}


IMPL_TEST_FNC(SHAREDTOSHARED)
{
	PSheet A = NewSheet();
	PSheet B = NewSheet(*A);	
}