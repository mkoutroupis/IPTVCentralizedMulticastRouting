#ifndef LOGGER_H_
#define LOGGER_H_

// '_vsnprintf': This function or variable may be unsafe
#pragma warning(disable:4996)

#include <ostream>
#include <fstream>
#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/optional.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/value_extraction.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include "LogLevel.h"
#include "Constants.h"
#include "MessageAction.h"
#include <boost/log/support/date_time.hpp>

namespace IPTVCommon {

	namespace logging = boost::log;
	namespace src = boost::log::sources;
	namespace expr = boost::log::expressions;
	namespace sinks = boost::log::sinks;
	namespace attrs = boost::log::attributes;

	using namespace std;

	BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string);
	BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", LogLevel);
	BOOST_LOG_ATTRIBUTE_KEYWORD(timeline, "Timeline", attrs::timer::value_type)

	class Logger
	{
		typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;

		boost::shared_ptr<text_sink> _error_sink;
		boost::shared_ptr<text_sink> _stat_sink;

		logging::attribute_set::iterator _timer_attr;

		logging::sources::severity_logger_mt<LogLevel> _lg;

		Logger();
		~Logger(){}

		Logger(Logger const&) = delete;              // Don't Implement
		void operator=(Logger const&) = delete; // Don't implement

		void _InitializeLog();
		void _InitializeStatistics();
	public:
		static Logger& Current(){
			static Logger instance; // Guaranteed to be destroyed.		
			return instance;
		}

		void StartTimer(){
			_timer_attr = _lg.add_attribute("Timeline", attrs::timer()).first;
		}

		void StopTimer(){
			_lg.remove_attribute(_timer_attr);
		}

		void LogError(const string& error_message);

		void LogStat(unsigned int node_id, const string& timestamp, MessageAction action, const string& operation);

		void LogDebug(const string& info_message);
	};
}
#endif

