#include "Client.h"
#include "Constants.h"
#include "UniverseFileParser.h"
#include <boost/asio.hpp>

namespace boost_io = boost::asio;

using namespace std;
using namespace IPTVCommon;

int main()
{
	boost_io::io_service io_service;

	auto uParser = new UniverseFileParser(Constants::UNIVERSE_FILE, Constants::UNIVERSE_NEIGHBOR_SEPARATOR, Constants::UNIVERSE_NEIGHBOR_FORMAT);
	uParser->Validate();
	uParser->Parse();

	vector<set<unsigned int>> examples = Utils::GetNFromSet(uParser->GetUniverse(), 10, Constants::SAMPLE_NODES);


	int numberOfExamples = 0;
	set<Channel> channels;
	Channel	channel = None;
	for (vector<set<unsigned int>>::iterator example_it = examples.begin(); example_it!= examples.end(); example_it++)
	{
		numberOfExamples++;
		string debugMessage = string("Example Number: ") + to_string(numberOfExamples);
		IPTVCommon::Logger::Current().LogDebug(debugMessage);

		srand(time(NULL));

		while (true)
		{
			int channelId = rand() % Constants::CLIENT_RANDOM_NUMBER_OF_CHANNELS;
			channel = static_cast<Channel>(channelId);

			auto exists = channels.find(channel);
			if (exists == channels.end())
			{
				break;
			}
		}

		for (set<unsigned int>::iterator node_it = example_it->begin(); node_it != example_it->end(); node_it++)
		{
			/*clock_t start_time = clock();
			clock_t end_time = sec * 1000 + start_time;
			while (clock() != end_time);*/

			std::chrono::milliseconds duration(500);
			std::this_thread::sleep_for(duration);

			debugMessage = string("Node Selected: ") + to_string(*node_it);
			IPTVCommon::Logger::Current().LogDebug(debugMessage);

			Client c1(io_service, Constants::SERVER_ADDRESS, Constants::SERVER_PORT);
			c1.SetParameters(Constants::CLIENT_DELAY, Constants::CLIENT_RANDOM_NUMBER_OF_MESSAGE_TYPES, Constants::CLIENT_RANDOM_NUMBER_OF_CHANNELS, Constants::CLIENT_RANDOM_NUMBER_OF_NEIGHBORS, Constants::CLIENT_RANDOM_MAX_NUMBER_FOR_NEIGHBORS);
			c1.SetNodeId(*node_it);
			c1.SetMessageType(3);
			c1.SetChannel(channel);
			c1.Run(1);
		}

		channels.insert(channel);		
		
	}

	
	
	system("PAUSE");
}