#ifndef SUBSCRIPTIONHOLDER_H_
#define SUBSCRIPTIONHOLDER_H_

#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <memory>
#include "Channel.h"
#include "UniverseNode.h"
#include "Logger.h"

using namespace std;
using namespace IPTVCommon;

class SubscriptionHolder: public enable_shared_from_this<SubscriptionHolder>
{
	map<Channel, set<UniverseNode>> _subscriptions;
public:
	shared_ptr<SubscriptionHolder> getptr() {
		return shared_from_this();
	}

	SubscriptionHolder() {
		IPTVCommon::Logger::Current().LogDebug("SubscriptionHolder instantiated successfully.");
	}
	~SubscriptionHolder() {
		IPTVCommon::Logger::Current().LogDebug("SubscriptionHolder deleted successfully.");
	}

	void Subscribe(Channel channel, UniverseNode& node);
	void UnSubscribe(Channel channel, UniverseNode& node);
	void UnSubscribe(UniverseNode& node);
	void Clear(){
		_subscriptions.clear();
	}

	set<UniverseNode> GetNodesForChannel(const Channel channel){
		auto it = _subscriptions.find(channel);
		if (it == _subscriptions.end()) return set<UniverseNode>();

		return it->second;
	}

	int GetNodeCountForChannel(const Channel channel){
		set<UniverseNode> nodes = GetNodesForChannel(channel);
		return nodes.size();
	}

	void Print(){
		cout << "************* SubscriptionHolder *************" << '\n';

		cout << "*Channels " << _subscriptions.size() << '\n';

		map<Channel, set<UniverseNode>>::iterator it;
		unsigned int index;
		for ( it = _subscriptions.begin(), index = 0; it != _subscriptions.end(); ++it, ++index) {
			cout << index + 1 << ": \"" << it->first << "\"\n";
		}

		cout << "*Subscribers \n";
		for (it = _subscriptions.begin(), index = 0; it != _subscriptions.end(); ++it, ++index) {
			cout << "\"" << it->first << "\" : ";
			set<UniverseNode> nodes = it->second;
			for (set<UniverseNode>::iterator nodes_it = nodes.begin(); nodes_it != nodes.end(); ++nodes_it){			
				cout << nodes_it->GetId() << " ";
			}
			cout << "\n";
		}
		cout << "/************* SubscriptionHolder *************/" << '\n';
	}
};

#endif