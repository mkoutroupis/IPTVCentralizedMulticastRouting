#ifndef LOGLEVEL_H_
#define LOGLEVEL_H_

#include <ostream>
#include <fstream>
#include <string>

namespace IPTVCommon {

	using namespace std;

	enum LogLevel
	{
		normal,
		notification,
		warning,
		error,
		critical
	};

	// The operator puts a human-friendly representation of the severity level to the stream
	inline ostream& operator<< (ostream& strm, LogLevel level)
	{
		static const char* strings[] =
		{
			"Normal",
			"Notification",
			"Warning",
			"Error",
			"Critical"
		};

		if (static_cast<size_t>(level) < sizeof(strings) / sizeof(*strings))
			strm << strings[level];
		else
			strm << static_cast<int>(level);

		return strm;
	}
}

#endif
