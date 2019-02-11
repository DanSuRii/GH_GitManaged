#pragma once

namespace NS_DPLOG
{


	class ILogger
	{
	public:
		virtual void WriteLine(std::string&) = 0;
	};
	using PILogger = std::shared_ptr<ILogger>;


	class Logger
	{
		Logger();
	public:
		virtual ~Logger();

		static Logger& GetInstance()
		{
			static Logger _inst;
			return _inst;
		}

		void WriteLine(std::string&&);
	private:
		void cbThreadWriter();
		//std::list< std::string > strReserved;
		//std::list< std::string > strToWrite;
		std::list< PILogger >		_contLogger;

		std::mutex					_mtxQueue;
		std::queue< std::string >	_queueStrToWrite;
		//Good code first, performance last.
		//std::list< std::string >	_queueStrToWrite;
		std::atomic_bool			bExit = false;

		std::thread					_thWriter;
	};


} // NS_DPLOG

template< class ...Args >
void Log(Args&& ... args)
{
	std::stringstream sstr;

	//a for aggregator
	const int a[]{ (sstr << args, 0) ... };
	static_cast<void>(a);

	auto& logger = NS_DPLOG::Logger::GetInstance();
	logger.WriteLine(sstr.str());
}


#define LOG_FN( ... ) Log( __FUNCSIG__ , __VA_ARGS__ ) 
#define CHECK_RETURN( cond, ... ) if( (cond) ){ LOG_FN( ", ", __FILE__, "(", __LINE__ ,") : " , #cond , ",  " , __VA_ARGS__ ); return ; }