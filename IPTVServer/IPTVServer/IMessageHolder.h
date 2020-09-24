#ifndef IMESSAGEHOLDER_H_
#define IMESSAGEHOLDER_H_

#include <iostream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "Message.h"
#include "UniverseFileParser.h"
#include "UniverseHolder.h"
#include "SubscriptionHolder.h"
#include "Constants.h"
#include "Utils.h"

namespace boost_generic = boost;
namespace boost_io = boost::asio;
namespace boost_ip = boost::asio::ip;

using namespace std;
using namespace IPTVCommon;

class IMessageHolder {
protected:
	boost_ip::udp::socket* _socket = nullptr;
	boost_ip::udp::endpoint _remote_endpoint;
	boost_generic::array<char, 56> recv_buf;
	boost_generic::array<char, 56> send_buf;

	bool _shouldCalculateShortestPaths = false;
	unsigned int _minSubcribersForCalculatingShortestPaths = 0;

	UniverseFileParser _u_parser;
	UniverseHolder _u_holder;
	SubscriptionHolder _sub_holder;

	virtual bool _DoWorkBasedOnMessage(Message& m){ return true; }
	virtual Message _GetReplyMessage(Message& m, bool calculateFlag){
		return m;
	}
	virtual void _StartReceive(){}
	virtual void _ReceiveMessage(const boost_generic::system::error_code& error, size_t bytes_transferred){}
	virtual void _SendMessage(boost_generic::array<char, 56> message){}
public:
	IMessageHolder(boost_io::io_service& io_service, unsigned short port, UniverseFileParser& universeFileParser, UniverseHolder& universeHolder, SubscriptionHolder& subscriptionHolder){
	}
	~IMessageHolder(){
		delete _socket;

		IPTVCommon::Logger::Current().LogDebug("MessageHolder deleted successfully.");
	}

	//void Run();

	void SetUniverseHolder(UniverseHolder& universeHolder){
		_u_holder = universeHolder;
	}

	void SetSubscriptionHolder(SubscriptionHolder& subcriptionholder){
		_sub_holder = subcriptionholder;
	}

	void SetNumberForCalculation(unsigned int minSubcribers){
		_minSubcribersForCalculatingShortestPaths = minSubcribers;
	}
};

#endif