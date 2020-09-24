#ifndef SERVER_H_
#define SERVER_H_

#include "Constants.h"
#include "UniverseHolder.h"
#include "UniverseFileParser.h"
#include "SubscriptionHolder.h"
#include "MessageHolder.h"
#include "MessageHolderForStats.h"
#include "Logger.h"
#include <boost/asio.hpp>

using namespace std;
using namespace IPTVCommon;

namespace boost_io = boost::asio;

class Server
{
	UniverseFileParser _u_parser;
	UniverseHolder _u_holder;
	SubscriptionHolder _sub_holder;
	IMessageHolder* _m_holder;

	boost_io::io_service _io_service;

	bool _run = true;
public:
	Server();
	~Server();

	void Start();
	void Stop();
	void Pause() {
		_run = false;
	}
	void Reset();
};

#endif

