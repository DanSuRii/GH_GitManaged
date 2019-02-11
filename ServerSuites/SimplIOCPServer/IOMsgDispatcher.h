#pragma once

#include <typeindex>
#include <unordered_map>
#include <functional>

namespace NS_DPNET
{

	template< class _MyT >
	class IOMsgDispatcher
	{
	public:
		using _MsgDispatcherT = IOMsgDispatcher<_MyT>;
		using MyFunctionT = std::function< void(_MyT*, void*, DWORD) >;

		template< class _FnT >
		void Regist(std::type_index idx, _FnT function)
		{
			_mapCaller[idx] = function;
		}

		auto GetFn(std::type_index idx)
		{
			MyFunctionT invalidFn;
			auto iter = _mapCaller.find(idx);
			if (iter == _mapCaller.end())
			{
				return invalidFn;
			}

			return iter->second;
		}
	private:
		std::unordered_map< std::type_index, MyFunctionT > _mapCaller;
	};

}
