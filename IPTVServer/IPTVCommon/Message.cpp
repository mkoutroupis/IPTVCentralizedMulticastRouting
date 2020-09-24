#include "Message.h"

void IPTVCommon::Message::Decode(boost::array<char, 56> buffer, IPTVCommon::Message& m)
{
	char c_nodeId[4];
	char c_action[4];
	char c_channel[4];

	c_nodeId[0] = buffer.elems[0];
	c_nodeId[1] = buffer.elems[1];
	c_nodeId[2] = buffer.elems[2];
	c_nodeId[3] = buffer.elems[3];
	c_action[0] = buffer.elems[4];
	c_action[1] = buffer.elems[5];
	c_action[2] = buffer.elems[6];
	c_action[3] = buffer.elems[7];
	c_channel[0] = buffer.elems[8];
	c_channel[1] = buffer.elems[9];
	c_channel[2] = buffer.elems[10];
	c_channel[3] = buffer.elems[11];

	m._nodeId = Utils::ConvertBytes2Int(c_nodeId);
	m._action = static_cast<MessageAction>(Utils::ConvertBytes2Int(c_action));
	m._channel = static_cast<Channel>(Utils::ConvertBytes2Int(c_channel));

	memcpy(m._timestamp, &buffer.elems[12], 20);
	memcpy(m._options, &buffer.elems[32], 24);
}

void IPTVCommon::Message::Encode(Message& m, boost::array<char, 56>& buffer)
{
	vector<char> nodeIdArray = Utils::ConvertInt2Bytes(m._nodeId);
	vector<char> actionArray = Utils::ConvertInt2Bytes(m._action);
	vector<char> channelArray = Utils::ConvertInt2Bytes(m._channel);

	buffer.elems[0] = nodeIdArray.at(0);
	buffer.elems[1] = nodeIdArray.at(1);
	buffer.elems[2] = nodeIdArray.at(2);
	buffer.elems[3] = nodeIdArray.at(3);
	buffer.elems[4] = actionArray.at(0);
	buffer.elems[5] = actionArray.at(1);
	buffer.elems[6] = actionArray.at(2);
	buffer.elems[7] = actionArray.at(3);
	buffer.elems[8] = channelArray.at(0);
	buffer.elems[9] = channelArray.at(1);
	buffer.elems[10] = channelArray.at(2);
	buffer.elems[11] = channelArray.at(3);

	memcpy(&buffer.elems[12], m._timestamp, 20);
	memcpy(&buffer.elems[32], m._options, 24);
}

void IPTVCommon::Message::InitializeTimestamp(Message& m)
{
	time_t now = time(0);
	string s = boost::lexical_cast<string>(now);
	const char* c_timestamp = s.c_str();
	strncpy_s(&m._timestamp[0], 20, c_timestamp, 20);
}


IPTVCommon::Message::Message()
{
	_nodeId = 0;
	_action = Subscribe;
	_channel = Channel0;
}

IPTVCommon::Message::Message(unsigned int nodeId, MessageAction action, Channel channel) : IPTVCommon::Message::Message() {

	_nodeId = nodeId;
	_action = action;
	_channel = channel;
}

IPTVCommon::Message::Message(unsigned int nodeId, MessageAction action, Channel channel, std::string timestamp) : IPTVCommon::Message::Message(nodeId, action, channel) {

	const char* c_timestamp = timestamp.c_str();
	strncpy_s(&_timestamp[0], 20, c_timestamp, 20);
}

IPTVCommon::Message::Message(unsigned int nodeId, MessageAction action, Channel channel, std::string timestamp, std::string options) : IPTVCommon::Message::Message(nodeId, action, channel, timestamp) {

	SetOptions(options.c_str());
}



