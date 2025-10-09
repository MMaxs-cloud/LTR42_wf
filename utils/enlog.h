/*
Template to write any data to log file.
Copyright 2005 by Victor Derevyanko, wingspan@yandex.ru

$Id: enlog.h 99 2005-10-28 07:51:41Z dv $
*/


#ifndef ENLOG_H
#define ENLOG_H_

#include <windows.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace Shims {
	template<class T>
	inline std::basic_stringstream<char>& place_message_to_stream(std::basic_stringstream<char>& Stream, T const& Value) {
		Stream << Value;
		return Stream;
	}

	//если необходимо значени€ определенного типа выводить в stream как-то хитро
	//необходиомо где-то в проекте объ€влить специализацию
	//template<> inline
	//std::basic_stringstream<char> Ext::Shims::place_message_to_stream(std::basic_stringstream<char> &Stream, MyType Value)
	//{ //!TODO };
}

namespace rt {
	namespace Private
	{
	
		struct DefaultChecker
		{	//to fix all events
			template <class T>
			static bool IsEventNeedToBeFixed(const T&) {	
				return true;
			}
		};

		struct DefaultLogger
		{	//to write events to stdout
			template <class T>
			static void Output(const T& level, std::string const & message) {
				std::cout << level << " "  << message << std::endl;
			}
		};

	}

// уже объ€влена в enforcer
// 	namespace Shims {
// 		template<class T> inline 
// 			std::basic_stringstream<char>& place_message_to_stream(std::basic_stringstream<char> &Stream, T const& Msg) 
// 		{
// 			Stream << Msg << " ";
// 			return Stream;
// 		}
// 	}

template<typename Ref, typename L, typename P, typename R>
class Enlogger
{
public:
	Enlogger(L level, Ref t) 
		: m_t(t)
		, m_level(level)
		, m_bFix(P::IsEventNeedToBeFixed(level))
    {
		if (m_bFix) operator()(t);
    }
	Enlogger(Enlogger const& E) {
		m_t = E.m_t;
		m_level = E.m_level;
		m_bFix = E.m_bFix;
		m_msg = E.m_msg;
	}

	Ref operator*() const
	{
		if (m_bFix) R::Output(m_level, m_msg.str().c_str());
		return m_t;
	}

    template <class MsgType>
    Enlogger& operator()(const MsgType& msg)
    {
        if (m_bFix) 
        {	//append parameters to original message
			Shims::place_message_to_stream(m_msg, msg);
			Shims::place_message_to_stream(m_msg, " ");
        }
        return *this;
    }

private:
	Ref m_t;
    L m_level;
    mutable std::basic_stringstream<char> m_msg;
    bool m_bFix;
};

template <class P, class R, typename T, typename Level>
inline Enlogger<const T&, Level, P, R> Make_Enlogger(Level const& level, const T& t)
{
    return Enlogger<const T&, Level, P, R>(level, t);
}

template <class P, class R, typename T, typename Level>
inline Enlogger<T&, Level, P, R> Make_Enlogger(Level const& level, T& t)
{
    return Enlogger<T&, Level, P, R>(level, t);
}


#define STRINGIZE(expr) STRINGIZE_HELPER(expr)
#define STRINGIZE_HELPER(expr) #expr

/************************************************************************/
/* ќбъ€влени€ макросов следует настроить под нужды программы и скопировать в stdafx.h

#define ENLOGN(level, exp) \
*Ext::Make_Enlogger<Ext::Private::DefaultChecker, Ext::Private::DefaultLogger>(level, (exp))

#define ENLOG(exp) ENLOGN(0, exp)	//default level

*/
/************************************************************************/

}; //Ext
#endif

