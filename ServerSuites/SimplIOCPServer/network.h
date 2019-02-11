#pragma once

namespace NS_DPNET
{
	class Network
	{
	public:
		enum ESTATE
		{
			ESTAT_INVALID = -100,

			ESTAT_DESTROY = -2,
			ESTAT_INVALID = -1,
			ESTAT_IDLE = 0,
			ESTAT_AVAILABLE,

			ESTAT_CNT,
		};

		Network();
		virtual ~Network();
		
		virtual bool Listen(std::string strPort) = 0;

		inline ESTATE GetStatus() { return eState; }
		
#if false // it does not needed
		template< ESTATE eState >
		struct EnumStr
		{
			std::string GetEnumStr(ESTATE eStr) { return "Str Doesnot Exists" + std::to_string(eStr); }
		};

		template< ESTATE eState > std::string GetEnumStr(ESTATE eStr) { return "Str Doesnot Exists" };
		template<  > std::string GetEnumStr<ESTAT_DESTROY>(ESTATE eStr) { return "ESTAT_DESTROY"; }
		template<  > std::string GetEnumStr<ESTAT_DESTROY>(ESTATE eStr) { return "ESTAT_DESTROY"; }
		template<  > std::string GetEnumStr<ESTAT_DESTROY>(ESTATE eStr) { return "ESTAT_DESTROY"; }
		template<  > std::string GetEnumStr<ESTAT_DESTROY>(ESTATE eStr) { return "ESTAT_DESTROY"; }
		template<  > std::string GetEnumStr<ESTAT_DESTROY>(ESTATE eStr) { return "ESTAT_DESTROY"; }
		template<  > std::string GetEnumStr<ESTAT_INVALID>(ESTATE eStr) { return "ESTAT DOES NOT EXISTS"; }



		std::string GetEnumStr(ESTATE eStr)
		{
			return GetEnumStr<ESTAT_CNT>();
		}

#endif // false // it does not needed

	protected:
		ESTATE eState = ESTAT_INVALID;
	};
	using PNetwork = std::shared_ptr< Network >;
}

