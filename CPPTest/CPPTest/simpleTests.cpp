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

#include <list>
using TestCont = std::list<int>;
class listTestRepo
{
public:

	void AddRange(TestCont&& rhs)
	{
		_cont.splice(_cont.end(), rhs);
	}

	void AddRange(TestCont& rhs)
	{
		_cont.insert(_cont.end(), rhs.begin(), rhs.end());
	}


	void AddRange__(TestCont rhs)
	{
		_cont.insert(_cont.end(), rhs.begin(), rhs.end());
	}

private:
	TestCont _cont{1,2,3,4,5};
};

IMPL_TEST_FNC(LIST_SPLICE_INSERT)
{
	TestCont localList{6,7,8,9};

	listTestRepo repo;

	repo.AddRange( localList );

	repo.AddRange( {7,8,9,10,11,12,13 } );
}

//void WriteLog(std::string strToWrite)
//{
//	std::cout << strToWrite << std::endl;
//}

template< class ...Args >
void WriteLogArgs(Args ...args)
{
	std::stringstream strToPrint;

	 //work Correctly
	const int a[] = { (strToPrint << args, 0) ... };
	static_cast<void>(a);

	/* // It doesnt work
		for (const auto val : { args... })
		{
			strToPrint << val;
		}	
	*/
	

	//{(strToPrint << args) ...}; // It doesnt work

	WriteLog(strToPrint.str());
}

/*
	//It doesnt work
	template< class T >
	void AddToStream( std::stringstream& sstream, T&& rhs )
	{
		sstream << rhs;
	}

	template< class ...Args >
	void WriteArgsWithFunc(Args ... args)
	{
		std::stringstream strToPrint;

		const int a[] = { ( AddToStream(args), 0 ) ... };
		static_cast<void>(a);

		WriteLog(strToPrint.str());
	}
*/

IMPL_TEST_FNC(VARIDARLIC_STRSTREAM)
{
	WriteLogArgs( "Init failed with code: ", -1, ", Possible to write: ", -2, " ", 0.007f );
	//WriteArgsWithFunc("Init failed with code: ", -1, ", Possible to write: ", -2, " ", 0.007f);
}


#include <winsock2.h>
#include <cstdio>

enum EIO_OPERATION
{
	IoAccept,
	IoRead,
	IoWrite,

	IO_CNT,
};


struct IOCtx
{
	WSAOVERLAPPED _overlapped;
	char _buffer[4096];
	WSABUF _wsaBuf;
	int nTotalBytes;
	int nSentBytes;

	EIO_OPERATION ioOperation;

	virtual void DoPrint() {}

	IOCtx()
		: _overlapped{0}
		, _buffer{ 0 }, _wsaBuf{0}, nTotalBytes(0), nSentBytes(0)
		, ioOperation( EIO_OPERATION::IO_CNT )
	{

	}
};

constexpr IOCtx* GetIoctxFromOverlappedPtr(LPWSAOVERLAPPED ptrOverlap) //it doesnt guarntee that would be member of IOCtx
{
	return reinterpret_cast<IOCtx*>(reinterpret_cast<char*>(ptrOverlap) - (int)(&(((IOCtx*)nullptr)->_overlapped)));
}

constexpr IOCtx* GetIoctxFromWsabufPtr(WSABUF* ptrWSABuf)
{
	return reinterpret_cast<IOCtx*>(reinterpret_cast<char*>(ptrWSABuf) - (int)(&(((IOCtx*)nullptr)->_wsaBuf)));
}

struct IOCtxInherit : public WSAOVERLAPPED
{
	char _buffer[4096];
	WSABUF _wsaBuf;
	int nTotalBytes;
	int nSentBytes;

	EIO_OPERATION ioOperation;

	virtual void DoPrint() {}

	IOCtxInherit()
		: WSAOVERLAPPED{0}
		, _buffer{ 0 }, _wsaBuf{ 0 }, nTotalBytes(0), nSentBytes(0)
		, ioOperation(EIO_OPERATION::IO_CNT)
	{

	}
};


IMPL_TEST_FNC(GET_STRUCTPTR_FROM_MEMBER_PTR)
{
	{
		IOCtx ioCtxt;
		fprintf(stdout, "CtxAddr = %p,", &ioCtxt);

		LPWSAOVERLAPPED pOverlapped = &(ioCtxt._overlapped);
		fprintf(stdout, "\n Overlapped = %p - Ctxt from Overlapped= %p ", pOverlapped, GetIoctxFromOverlappedPtr(pOverlapped) );

		//fprintf(stdout, "\n Overlapped = %p - Ctxt static cast from Overlapped= %p ", pOverlapped, static_cast<IOCtx*>(pOverlapped) );

		WSABUF* pWSABuff = &(ioCtxt._wsaBuf);
		fprintf(stdout, "\n wsaBuf = %p - Ctxt from wsaBuf= %p ", pWSABuff, GetIoctxFromWsabufPtr( pWSABuff ));
		fprintf(stdout, "\n");

	}

	{
		IOCtxInherit inheritCtxt;
		fprintf(stdout, "\n CtxAddr = %p,", &inheritCtxt);
		
		LPWSAOVERLAPPED pOverlapped = static_cast<LPWSAOVERLAPPED>(&inheritCtxt);
		fprintf(stdout, "\n Overlapped = %p ", pOverlapped );

		IOCtxInherit* pInheritCtxt = static_cast<IOCtxInherit*>(pOverlapped);
		fprintf(stdout, "\n pInheritCtxt = %p", pInheritCtxt);
		fprintf(stdout, "\n");
	}


}