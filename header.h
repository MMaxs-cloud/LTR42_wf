#include <string>
#include <functional>

#pragma once

#define ENLOGN(level, exp) \
*rt::Make_Enlogger<rt::Private::DefaultChecker, rt::RTEngineLogger>(level, (exp))
#define ENLOG(exp) ENLOGN(0, exp)

namespace rt {	
	struct RTEngineLogger
	{	//to write events to stdout
		template <class T>
		static void Output(const T& level, std::string const& message) {
			write_to_log(message);
		}
		static void write_to_log(std::string const& str);
	};

	enum LogLevel { Error, Debug, Warning, Info };
	typedef std::function<void(std::string const&, LogLevel)> tlog_function;


	/// примитивная обертка над вызовом функции tlog_function
	class Logger {
	public:
		Logger();
		void init(rt::tlog_function func) { _Func = func; }

		void error(std::string const& msg) {
			if (_Func != nullptr) {
				_Func(msg, rt::Error);
			}
		}
		void debug(std::string const& msg) {
			if (_Func != nullptr) {
				_Func(msg, rt::Debug);
			}
		}
		void warning(std::string const& msg) {
			if (_Func != nullptr) {
				_Func(msg, rt::Warning);
			}
		}
		void info(std::string const& msg) {
			if (_Func != nullptr) {
				_Func(msg, rt::Info);
			}
		}
	private:
		rt::tlog_function _Func;
	};
}
