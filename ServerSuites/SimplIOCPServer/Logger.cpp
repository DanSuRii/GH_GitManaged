#include "pch.h"

#include <string>
#include <sstream>

#include "Logger.h"

#include <iostream>
#include <ctime>

#include <boost\chrono.hpp>

// ILog instantination
class ConsoleLog : public ILogger
{
public:
	ConsoleLog()
	{
	}
	~ConsoleLog()
	{
	}

private:

	// Inherited via ILogger
	virtual void WriteLine(std::string& strToWrite) override
	{
		std::cout << strToWrite << std::endl;
	}

};

Logger::Logger()
	: _contLogger{ New<ConsoleLog>() }
	, _thWriter( std::bind( &Logger::cbThreadWriter, this ) )
{
	
}


Logger::~Logger()
{
	bExit = true;	

	_thWriter.join();
}

void Logger::WriteLine(std::string&& strToWrite )
{
#if true
	auto now = boost::chrono::system_clock::now();

	std::stringstream sstream;
	sstream << now << " >> " << strToWrite;


	std::lock_guard<decltype(_mtxQueue)> grd(_mtxQueue);
	_queueStrToWrite.push_back(std::move(sstream.str()));
#else
	std::lock_guard<decltype(_mtxQueue)> grd(_mtxQueue);
	_queueStrToWrite.push_back(std::move(strToWrite));
#endif // false 
}

void Logger::cbThreadWriter()
{
	using namespace std::chrono_literals;
	while (false == bExit)
	{
		if ( 0 == _queueStrToWrite.size()) {
			std::this_thread::sleep_for( 500ms );
		} else {
			std::string strToWrite;
			{
				std::lock_guard<decltype(_mtxQueue)> grd(_mtxQueue);
				strToWrite = _queueStrToWrite.front();
				_queueStrToWrite.pop_front();
			}
			
			for (auto cur : _contLogger) {
				cur->WriteLine(strToWrite);
			}
		}

	}
}
