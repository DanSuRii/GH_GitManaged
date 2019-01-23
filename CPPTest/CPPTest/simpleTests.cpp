#include "pch.h"

#include <sstream>

void WriteLog(std::string strToWrite)
{
	std::cout << strToWrite << std::endl;
}

IMPL_TEST_FNC(SSTREAMLOG)
{
	int nRet(-32);

	WriteLog((std::stringstream() << "Failed with : " << nRet).str());
}