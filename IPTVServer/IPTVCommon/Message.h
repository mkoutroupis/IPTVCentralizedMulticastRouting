#ifndef MESSAGE_H_
#define MESSAGE_H_

#pragma warning(disable:4996)

#include "Channel.h"
#include "Utils.h"
#include "MessageAction.h"
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/date_facet.hpp>
#include <sstream>
#include <string>
#include <time.h>

namespace IPTVCommon {

	using namespace std;
	
	class Message
	{
		unsigned int _nodeId = 0;
		MessageAction _action;
		Channel _channel;
		char _timestamp[20] = {};
		char _options[24] = {};

	public:
		Message();
		
		Message(unsigned int nodeId, MessageAction action, Channel channel);

		Message(unsigned int nodeId, MessageAction action, Channel channel, string timestamp);

		Message(unsigned int nodeId, MessageAction action, Channel channel, string timestamp, string options);

		~Message()
		{

		}

		const unsigned int GetNodeId()
		{
			return _nodeId;
		}

		const MessageAction GetAction()
		{
			return _action;
		}

		const Channel GetChannel()
		{
			return _channel;
		}

		const char* GetTimestamp()
		{
			return _timestamp;
		}

		const char* GetOptions()
		{
			return _options;
		}

		void SetOptions(const char* options)
		{
			if (options == nullptr) return;

			memcpy(&_options[0], options, 24);
		}

		static void Decode(boost::array<char, 56> buffer, Message& m);

		static void Encode(Message& m, boost::array<char, 56>& r);

		static void InitializeTimestamp(Message& m);
	};
}
#endif