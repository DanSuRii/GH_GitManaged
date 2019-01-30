#include "pch.h"

#include <string>
#include <sstream>

#include "Logger.h"

#include <iostream>
#include <ctime>

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

template<class ...Args>
void Log(Args && ... args)
{
	std::stringstream sstr;

	//a for aggregator
	const int a[]{ ( sstr << args, 0 ) ... };
	static_cast<void>(a);

	auto logger =  Logger::GetInstance();
	logger.WriteLine( sstr.str() );
}

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
	auto now = std::chrono::system_clock::now();
	std::string strQueue = strToWrite;

	auto time = std::chrono::system_clock::to_time_t(now);
	strQueue.insert(0, std::string(std::ctime(&time)) + " >> ");

	std::lock_guard<decltype(_mtxQueue)> grd(_mtxQueue);
	_queueStrToWrite.push_back( std::move( strQueue ) );
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
