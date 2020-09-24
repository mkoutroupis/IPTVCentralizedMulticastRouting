#include "MessageHolderForStats.h"

MessageHolderForStats::MessageHolderForStats(boost_io::io_service& io_service, unsigned short port, UniverseFileParser& universeFileParser, UniverseHolder& universeHolder, SubscriptionHolder& subscriptionHolder,unsigned int minSubcribers)
	: IMessageHolder(io_service, port, universeFileParser, universeHolder, subscriptionHolder) {

	try
	{
		_u_parser = universeFileParser;
		_u_holder = universeHolder;
		_sub_holder = subscriptionHolder;

		IMessageHolder::SetNumberForCalculation(minSubcribers);

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

MessageHolderForStats::~MessageHolderForStats(){	
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

bool  MessageHolderForStats::_DoWorkBasedOnMessage(Message& m)
{
	UniverseNode clientNode(m.GetNodeId());
	bool _shouldCalculate = false;


	if (m.GetAction() == Subscribe)
	{
		Channel channel = m.GetChannel();

		unsigned int numberOfSubscribers =  _sub_holder.GetNodeCountForChannel(channel);
		_sub_holder.Subscribe(channel, clientNode);
		if (numberOfSubscribers + 1 >= _minSubcribersForCalculatingShortestPaths){
			_shouldCalculate = true;
		}
		else
		{
			_shouldCalculate = false;
		}
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

	return _shouldCalculate;
}

Message  MessageHolderForStats::_GetReplyMessage(Message& m, bool _shouldCalculate)
{
	UniverseNode serverNode(Constants::MEET_SERVER_NODE_ID);
	
	vector<unsigned int> path;
	if (_shouldCalculate)
	{
		set<UniverseNode> channelSubscribers = _sub_holder.GetNodesForChannel(m.GetChannel());
		for (set<UniverseNode>::iterator it = channelSubscribers.begin(); it!= channelSubscribers.end(); it++)
		{
			IPTVCommon::Logger::Current().StartTimer();
			path = _u_holder.GetShortestPath(serverNode, *it);
		}
		IPTVCommon::Logger::Current().LogStat(m.GetNodeId(), m.GetTimestamp(), m.GetAction(), "Calculate Shortest Paths Completed");
		IPTVCommon::Logger::Current().StopTimer();
	}

	string options = Utils::Convert2String<unsigned int>(path);
	Message r(m.GetNodeId(), m.GetAction(), m.GetChannel(), m.GetTimestamp(), options);

	return r;
}

void MessageHolderForStats::_StartReceive()
{
	_socket->async_receive_from(boost_io::buffer(recv_buf), _remote_endpoint,
		boost_generic::bind(&MessageHolderForStats::_ReceiveMessage, this, boost_io::placeholders::error, boost_io::placeholders::bytes_transferred));
}

void MessageHolderForStats::_ReceiveMessage(const boost_generic::system::error_code& error, size_t bytes_transferred)
{
	if (!error || error == boost_io::error::message_size)
	{
		Message send_message;
		Message::Decode(recv_buf, send_message);

		const unsigned int nodeId = send_message.GetNodeId();
		const string timestamp = send_message.GetTimestamp();
		const MessageAction action = send_message.GetAction();

		bool _result = _DoWorkBasedOnMessage(send_message);
		
		Message reply_message = _GetReplyMessage(send_message,_result);
		Message::Encode(reply_message, send_buf);

		_socket->async_send_to(boost_io::buffer(send_buf), _remote_endpoint, boost_generic::bind(&MessageHolderForStats::_SendMessage, this, send_buf));
		//_socket->send_to(boost_io::buffer(send_buf), _remote_endpoint);

		_StartReceive();
	}
	else
	{

	}
}

void MessageHolderForStats::_SendMessage(boost_generic::array<char, 56> message)
{
	Message reply_message;
	Message::Decode(message, reply_message);
}