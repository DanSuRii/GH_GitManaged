#include "pch.h"

#include <string>
#include <sstream>

#include "Logger.h"

#include <iostream>
#include <ctime>

#include <boost\chrono.hpp>

namespace NS_DPLOG
{


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
		, _thWriter(std::bind(&Logger::cbThreadWriter, this))
	{

	}


	Logger::~Logger()
	{
		bExit = true;

		_thWriter.join();
	}

	void Logger::WriteLine(std::string&& strToWrite)
	{
#if true
		std::stringstream sstream;

#if true
		time_t curTime = ::time(nullptr);
		struct tm buf;
		localtime_s( &buf, &curTime);
		char timeStr[32]{ 0, };		
		asctime_s(timeStr, &buf);

		sstream << timeStr << " >> " << strToWrite;

#else
		auto now = boost::chrono::system_clock::now();

		auto strNow = boost::chrono::to_string(now);

		sstream << strNow << " >> " << strToWrite;
#endif


		std::lock_guard<decltype(_mtxQueue)> grd(_mtxQueue);
		_queueStrToWrite.push(std::move(sstream.str()));


#else
		std::lock_guard<decltype(_mtxQueue)> grd(_mtxQueue);
		_queueStrToWrite.push_back(std::move(strToWrite));
#endif // false 
	}

	void Logger::cbThreadWriter()
	{
		std::atomic_int nCnt = 0;
		using namespace std::chrono_literals;
		while (false == bExit || 0 != _queueStrToWrite.size())
		{
			if (0 == _queueStrToWrite.size()) {
				std::this_thread::sleep_for(500ms);
			}
			else {
				std::string strToWrite;
				{
					std::lock_guard<decltype(_mtxQueue)> grd(_mtxQueue);
					strToWrite = _queueStrToWrite.front();
					_queueStrToWrite.pop();
				}

				std::stringstream sstrWrite;
				sstrWrite << ++nCnt << "::" << strToWrite;
				std::string strWriteWithCnt( sstrWrite.str() );

				/*
				++nCnt;
				strWriteWithCnt.append( itos( nCnt ) );
				strWriteWithCnt += strToWrite;
				*/

				for (auto cur : _contLogger) {
					cur->WriteLine(strWriteWithCnt);
				}
			}
		}
	}
}
