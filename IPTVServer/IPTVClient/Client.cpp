#include "Client.h"

Client::Client(boost_io::io_service& io_service, string server_address, unsigned short server_port) : _socket(io_service)
{
	try
	{
		boost_ip::udp::resolver resolver(io_service);
		boost_ip::udp::resolver::query query(boost_ip::udp::v4(), server_address, boost_generic::lexical_cast<string>(server_port));
		_receiver_endpoint = *resolver.resolve(query);

		_socket.open(boost_ip::udp::v4());

	}
	catch (exception& e)
	{
		string exceptionMessage(e.what());
		string errorMessage = "Client error during instantiation. Exception: " + exceptionMessage;
		cerr << errorMessage << endl;
		throw errorMessage;
	}
}


void Client::_SendMessage()
{
	_CreateRandomMessage();

	_socket.send_to(boost_io::buffer(send_buf), _receiver_endpoint);

	_counter++;
}


void Client::Run(unsigned int times)
{
	try
	{

		while (_run && _counter< times)
		{
			/*chrono::milliseconds dura(_delay);
			this_thread::sleep_for(dura);*/

			_SendMessage();

			_ReceiveMessage();
		}

	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}
}

void Client::_CreateRandomMessage()
{
	Message m;

	srand(time(NULL));

	if (_message_type == 0)
	{
		_message_type = rand() % _max_message_types + 1;
	}

	if (_message_type == 1) //Add
	{
		int number_of_neighbors = rand() % _number_of_neighbors + 1;
		vector<unsigned int> neighbors;

		for (int i = 0; i < number_of_neighbors; i++)
		{
			//peek neightbor
			int neighborId = rand() % _max_neighbors + 1;
			neighbors.push_back(neighborId);
		}

		m = { _node_id, Add, Channel0 };
		string s_neightbors = IPTVCommon::Utils::Convert2String(neighbors);
		m.SetOptions(s_neightbors.c_str());
	}
	else if (_message_type == 2) //Delete
	{
		m = { _node_id, Delete, Channel0 };
	}
	else if (_message_type == 3)
	{
		if (_channel == None)
		{
			int channel = rand() % _max_channels;
			_channel = static_cast<Channel>(channel);
		}

		m = { _node_id, Subscribe,  _channel};
	}
	else if (_message_type == 4)
	{
		if (_channel == None)
		{
			int channel = rand() % _max_channels;
			_channel = static_cast<Channel>(channel);
		}

		m = { _node_id, UnSubscribe, _channel };
	}

    Message::InitializeTimestamp(m);
	Message::Encode(m, send_buf);
}

void Client::_ReceiveMessage()
{
	boost_generic::system::error_code error;
	size_t len = _socket.receive_from(boost_io::buffer(recv_buf), _receiver_endpoint, 0, error);

	if (error && error != boost_io::error::message_size)
		throw boost_generic::system::system_error(error);

	if (len != 0)
	{
		Message m;

		cout << "Client received: ";
		Message::Decode(recv_buf,m);
		cout << m.GetNodeId() << "\t" << m.GetAction() << "\t" << m.GetChannel() << "\t" << m.GetTimestamp() << "\t" << m.GetOptions();
		cout << "\n";
	}
}