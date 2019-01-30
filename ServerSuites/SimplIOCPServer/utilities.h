#pragma once

template< class T, class ... Args >
decltype(auto) New( Args&& ...args )
{
	return std::make_shared<T>(args...);
}