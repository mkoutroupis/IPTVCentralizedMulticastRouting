#ifndef MESSAGEHOLDERFORSTATS_H_
#define MESSAGEHOLDERFORSTATS_H_

#include "IMessageHolder.h"

using namespace std;
using namespace IPTVCommon;

class MessageHolderForStats : public IMessageHolder {
protected:
	bool _DoWorkBasedOnMessage(Message& m);
	Message _GetReplyMessage(Message& m, bool calculateFlag);
	void _StartReceive();
	void _ReceiveMessage(const boost_generic::system::error_code& error, size_t bytes_transferred);
	void _SendMessage(boost_generic::array<char, 56> message);
public:
	MessageHolderForStats(boost_io::io_service& io_service, unsigned short port, UniverseFileParser& universeFileParser, UniverseHolder& universeHolder, SubscriptionHolder& subscriptionHolder, unsigned int minSubcribers);
	~MessageHolderForStats();

	//void Run();
};

#endif