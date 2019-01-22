#pragma once

namespace NS_TEST
{
	class Test
	{
	public:
		virtual void Do() = 0;
	};

	template< std::uint32_t TName >
	class TestImpl : public Test
	{
	public:
		TestImpl(std::string strName) : _strName(strName) {}

		virtual void Do();
	private:

		std::string _strName;
	};

	class TestRepo
	{
	public:
		void DoAll();
	private:
		static std::shared_ptr<Test> _cont[];
	};

}
namespace detail {
	constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
	{
		return((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
	}
}

constexpr std::uint32_t operator"" _hash(char const* s, std::size_t count)
{
	return detail::fnv1a_32(s, count);
}



#define IMPL_TEST_FNC( x ) 	void NS_TEST::TestImpl<#x##_hash>::Do()
