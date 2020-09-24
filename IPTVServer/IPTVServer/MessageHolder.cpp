#include "MessageHolder.h"

MessageHolder::MessageHolder(boost_io::io_service& io_service, unsigned short port, UniverseFileParser& universeFileParser, UniverseHolder& universeHolder, SubscriptionHolder& subscriptionHolder)
	: IMessageHolder(io_service, port, universeFileParser, universeHolder, subscriptionHolder) {

	try
	{
		_u_parser = universeFileParser;
		_u_holder = universeHolder;
		_sub_holder = subscriptionHolder;

		boost_generic::system::error_code error;

		_socket = new boost_ip::udp::socket(io_service, boost_ip::udp::endpoint(boost_ip::udp::v4(), port));

		IPTVCommon::Logger::Current().LogDebug("MessageHolder instantiated successfully.");

		_StartReceive();
	}
	catch (exception& e)
	{
		string exceptionMessage(e.what());
		string errorMessage = "Message Holder error during instantiation. Exception: " + exceptionMessage;
		cerr << errorMessage << endl;
		throw errorMessage;
	}
}

MessageHolder::~MessageHolder(){
}

//void MessageHolder::Run()
//{
//	for (;;)
//	{
//		boost::system::error_code error;
//		size_t bytes_transferred = _socket->receive_from(boost_io::buffer(recv_buf), _remote_endpoint, 0, error);
//
//		_ReceiveMessage(error, bytes_transferred);
//	}
//}

bool  MessageHolder::_DoWorkBasedOnMessage(Message& m)
{
	UniverseNode clientNode(m.GetNodeId());

	if (m.GetAction() == Subscribe)
	{
		_sub_holder.Subscribe(m.GetChannel(), clientNode);
	}
	else if (m.GetAction() == UnSubscribe)
	{
		if (m.GetChannel() != -1)
		{
			_sub_holder.UnSubscribe(m.GetChannel(), clientNode);
		}
		else
		{
			_sub_holder.UnSubscribe(clientNode);
		}
	}
	else if (m.GetAction() == Add)
	{
		_u_holder.AddNode(clientNode);
		vector<unsigned int> neighborIds = Utils::Convert2Vector<unsigned int>(m.GetOptions());

		for (vector<unsigned int>::iterator it = neighborIds.begin(); it != neighborIds.end(); ++it)
		{
			UniverseNode neighborNode(*it);
			_u_holder.AddEdge(clientNode, neighborNode);
		}
	}
	else if (m.GetAction() == Delete)
	{
		_sub_holder.UnSubscribe(clientNode);
		_u_holder.RemoveNode(clientNode);
	}

	return true;
}

Message  MessageHolder::_GetReplyMessage(Message& m, bool calculateFlag)
{
	UniverseNode serverNode(Constants::MEET_SERVER_NODE_ID);
	UniverseNode clientNode(m.GetNodeId());

	string options;
	if (calculateFlag)
	{
		vector<unsigned int> path = _u_holder.GetShortestPath(serverNode, clientNode);
		options = Utils::Convert2String<unsigned int>(path);
	}
	Message r(m.GetNodeId(), m.GetAction(), m.GetChannel(), m.GetTimestamp(), options);

	return r;
}

void MessageHolder::_StartReceive()
{
	_socket->async_receive_from(boost_io::buffer(recv_buf), _remote_endpoint,
		boost_generic::bind(&MessageHolder::_ReceiveMessage, this, boost_io::placeholders::error, boost_io::placeholders::bytes_transferred));
}

void MessageHolder::_ReceiveMessage(const boost_generic::system::error_code& error, size_t bytes_transferred)
{
	if (!error || error == boost_io::error::message_size)
	{
		IPTVCommon::Logger::Current().StartTimer();

		Message send_message;
		Message::Decode(recv_buf, send_message);

		const unsigned int nodeId = send_message.GetNodeId();
		const string timestamp = send_message.GetTimestamp();
		const MessageAction action = send_message.GetAction();

		IPTVCommon::Logger::Current().LogStat(nodeId,timestamp , action, "Decode Message Completed");

		bool calculateFlag = _DoWorkBasedOnMessage(send_message);
		IPTVCommon::Logger::Current().LogStat(nodeId, timestamp, action, "Work Based On Message Completed");

		Message reply_message = _GetReplyMessage(send_message, calculateFlag);
		Message::Encode(reply_message, send_buf);
		IPTVCommon::Logger::Current().LogStat(nodeId, timestamp, action, "Reply Message Created");

		_socket->async_send_to(boost_io::buffer(send_buf), _remote_endpoint, boost_generic::bind(&MessageHolder::_SendMessage, this, send_buf));
		//_socket->send_to(boost_io::buffer(send_buf), _remote_endpoint);

		_StartReceive();
	}
}

void MessageHolder::_SendMessage(boost_generic::array<char, 56> message)
{
	Message reply_message;
	Message::Decode(message, reply_message);
	IPTVCommon::Logger::Current().LogStat(reply_message.GetNodeId(), reply_message.GetTimestamp(), reply_message.GetAction(), "Reply Message Send");
	IPTVCommon::Logger::Current().StopTimer();
}