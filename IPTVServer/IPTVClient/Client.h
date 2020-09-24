#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Message.h"
#include "Utils.h"
#include <chrono>
#include <thread>

namespace boost_generic = boost;
namespace boost_io = boost::asio;
namespace boost_ip = boost::asio::ip;
namespace boost_time = boost::posix_time;
namespace boost_thread = boost::this_thread;


using namespace std;
using namespace IPTVCommon;

class Client
{
	boost_ip::udp::socket _socket;
	boost_ip::udp::endpoint _receiver_endpoint;
	boost_generic::array<char, 56> recv_buf;
	boost_generic::array<char, 56> send_buf;

	bool _run = true;
	unsigned long _counter = 0;
	unsigned int _delay = 0;
	unsigned int _max_message_types = 0;
	unsigned int _max_channels = 0;
	unsigned int _number_of_neighbors = 0;
	unsigned int _max_neighbors = 0;

	unsigned int _node_id = 0;
	unsigned int _message_type = 0;
	Channel _channel = None;

	void _CreateRandomMessage(); 
	void _SendMessage();
	void _ReceiveMessage();
public:	
	Client(boost_io::io_service& io_service,string server_address, unsigned short server_port);
	~Client() {

	};

	void Run(unsigned int times);
	void Stop() { 
		Pause(); 
		_socket.close(); 
	}
	void Pause() { 
		_run = false; 
	}
	void Reset(){
		_counter = 0;
	}
	void SetParameters(unsigned int delay, unsigned int message_types, unsigned int channels, unsigned int number_of_neighbors, unsigned int max_neighbors){
		_delay = delay;
		_max_message_types = message_types;
		_max_channels = channels;
		_number_of_neighbors = number_of_neighbors;
		_max_neighbors = max_neighbors;
	}

	void SetNodeId(unsigned int node_id){
		_node_id = node_id;
	}

	void SetChannel(Channel channel){
		_channel = channel;
	}

	void SetMessageType(unsigned int message_type){
		_message_type = message_type;
	}


	const long GetCounter(){
		return _counter;
	}

	const unsigned int GetNodeId(){
		return _node_id;
	}
};

#endif