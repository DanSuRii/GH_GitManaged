#pragma once

namespace NS_DPNET
{
	class Network
	{
	public:
		Network();
		virtual ~Network();
	};
	using PNetwork = std::shared_ptr< Network >;
}

