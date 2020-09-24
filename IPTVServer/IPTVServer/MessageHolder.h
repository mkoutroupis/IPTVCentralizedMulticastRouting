#ifndef MESSAGEHOLDER_H_
#define MESSAGEHOLDER_H_

#include "IMessageHolder.h"

using namespace std;
using namespace IPTVCommon;

class MessageHolder : public IMessageHolder {
protected:
	bool _DoWorkBasedOnMessage(Message& m);
	Message _GetReplyMessage(Message& m, bool calculateFlag);
	void _StartReceive();
	void _ReceiveMessage(const boost_generic::system::error_code& error, size_t bytes_transferred);
	void _SendMessage(boost_generic::array<char, 56> message);
public:
	MessageHolder(boost_io::io_service& io_service, unsigned short port, UniverseFileParser& universeFileParser, UniverseHolder& universeHolder, SubscriptionHolder& subscriptionHolder);
	~MessageHolder();

	//void Run();

};

#endif