#include "pch.h"
#include "CTest.h"


#define DECL_TEST( x ) std::make_shared< TestImpl<#x##_hash> >( #x )	

namespace NS_TEST
{

	std::shared_ptr<Test> TestRepo::_cont[] = {
		DECL_TEST(HWORLD),
		DECL_TEST(WORLDTEST),
		DECL_TEST(SHAREDTOSHARED),
		DECL_TEST(SSTREAMLOG),
	};

	void TestImpl<"HWORLD"_hash >::Do() { std::cout << this->_strName << std::endl; }
	void TestImpl<"WORLDTEST"_hash>::Do() { std::cout << this->_strName << std::endl; }


	/*
	std::shared_ptr<Test> TestRepo::_cont[] = {
		std::make_shared< TestImpl<"HWORLD"_hash> >( "HWORLD" ),
		std::make_shared< TestImpl<"WORLD"_hash> > ( "WORLD" ),
	};
	*/


	void TestRepo::DoAll() {
		for (auto cur : _cont)
		{
			cur->Do();
		}
	}

}
