#include "SubscriptionHolder.h"

void SubscriptionHolder::Subscribe(Channel channel, UniverseNode& node){

	auto it = _subscriptions.find(channel);
	if (it == _subscriptions.end())
	{
		set<UniverseNode> node_set;
		node_set.insert(node);
		_subscriptions.insert(pair<Channel, set<UniverseNode>>(channel, node_set));
	}
	else
	{
		it->second.insert(node);
	}
}

void SubscriptionHolder::UnSubscribe(Channel channel, UniverseNode& node){
	auto it = _subscriptions.find(channel);
	if (it != _subscriptions.end())
	{
		it->second.erase(node);
	}
}

void SubscriptionHolder::UnSubscribe(UniverseNode& node){
	map<Channel, set<UniverseNode>>::iterator it;
	unsigned int index;
	for (it = _subscriptions.begin(), index = 0; it != _subscriptions.end(); ++it, ++index) {
		it->second.erase(node);
	}
}