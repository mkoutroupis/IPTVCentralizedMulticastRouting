#ifndef MESSAGEACTION_H_
#define MESSAGEACTION_H_

#include <ostream>
#include <fstream>
#include <string>

namespace IPTVCommon {

	using namespace std;

	enum MessageAction
	{
		Subscribe,
		UnSubscribe,
		Add,
		Delete
	};

	inline ostream& operator<< (ostream& strm, MessageAction action)
	{
		static const char* strings[] =
		{
			"Subscribe",
			"UnSubscribe",
			"Add",
			"Delete"
		};

		if (static_cast<size_t>(action) < sizeof(strings) / sizeof(*strings))
			strm << strings[action];
		else
			strm << static_cast<int>(action);

		return strm;
	}

}

#endif