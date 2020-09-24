#include "Logger.h"

IPTVCommon::Logger::Logger()
{
	_InitializeLog();
	_InitializeStatistics();
}

void IPTVCommon::Logger::_InitializeLog()
{
	_error_sink = boost::make_shared<text_sink>();

	_error_sink->locked_backend()->add_stream(boost::make_shared< std::ofstream >(IPTVCommon::Constants::LOG_FILE));

	_error_sink->locked_backend()->auto_flush(true);

	_error_sink->set_formatter(
		expr::stream
		<< expr::attr< unsigned int >("LineID") << IPTVCommon::Constants::LOG_SEPARATOR
		<< severity << IPTVCommon::Constants::LOG_SEPARATOR
		<< expr::smessage
	);

	_error_sink->set_filter(expr::has_attr(tag_attr) && tag_attr == "ERROR");

	logging::core::get()->add_sink(_error_sink);

	logging::add_common_attributes();
}

void IPTVCommon::Logger::_InitializeStatistics()
{
	_stat_sink = boost::make_shared<text_sink>();

	_stat_sink->locked_backend()->add_stream(boost::make_shared< std::ofstream >(IPTVCommon::Constants::STATISTICS_FILE));

	_stat_sink->locked_backend()->auto_flush(true);

	_stat_sink->set_formatter(
		expr::stream
		<< expr::attr< unsigned int >("LineID") << IPTVCommon::Constants::STATISTICS_SEPARATOR
		<< expr::attr< unsigned int >("NodeID") << IPTVCommon::Constants::STATISTICS_SEPARATOR
		<< expr::attr< string >("MessageTimestamp") << IPTVCommon::Constants::STATISTICS_SEPARATOR
		<< expr::attr< string >("MessageType") << IPTVCommon::Constants::STATISTICS_SEPARATOR
		<< setw(50) << expr::smessage << setfill(' ') << IPTVCommon::Constants::STATISTICS_SEPARATOR
		<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S") << IPTVCommon::Constants::STATISTICS_SEPARATOR
		<< expr::if_(expr::has_attr(timeline))
		[
			expr::stream << timeline
		]
	);

	_stat_sink->set_filter(expr::has_attr(tag_attr) && tag_attr == "STAT");

	logging::core::get()->add_sink(_stat_sink);

	logging::add_common_attributes();
}

void IPTVCommon::Logger::LogError(const string& error_message)
{
	BOOST_LOG_SCOPED_THREAD_TAG("Tag", "ERROR");

	BOOST_LOG_SEV(_lg, error) << error_message;
}

void IPTVCommon::Logger::LogDebug(const string& info_message)
{
	BOOST_LOG_SCOPED_THREAD_TAG("Tag", "ERROR");

	BOOST_LOG_SEV(_lg, normal) << info_message;

	_error_sink->flush();
}

void IPTVCommon::Logger::LogStat(unsigned int node_id, const string& timestamp, IPTVCommon::MessageAction action, const string& operation)
{
	BOOST_LOG_SCOPED_THREAD_TAG("Tag", "STAT");

	ostringstream oss;
	oss << action;
	const string message_type = oss.str();	

	BOOST_LOG_SCOPED_LOGGER_ATTR(_lg, "NodeID", attrs::constant<unsigned int>(node_id));
	BOOST_LOG_SCOPED_LOGGER_ATTR(_lg, "MessageTimestamp", attrs::constant<string>(timestamp));
	BOOST_LOG_SCOPED_LOGGER_ATTR(_lg, "MessageType", attrs::constant<string>(message_type));

	BOOST_LOG(_lg) << operation;

	_stat_sink->flush();
}