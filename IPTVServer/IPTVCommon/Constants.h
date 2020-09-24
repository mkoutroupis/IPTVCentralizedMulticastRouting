#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>
using namespace std;

namespace IPTVCommon {
	class Constants{

	public:
		static const string UNIVERSE_FILEPATH;
		static const string UNIVERSE_FILE;
		static const string UNIVERSE_FILENAME;
		static const string UNIVERSE_NEIGHBOR_SEPARATOR;
		static const string UNIVERSE_NEIGHBOR_FORMAT;

		static const string LOG_FILENAME;
		static const string LOG_FILEPATH;
		static const string LOG_FILE;
		static const string LOG_SEPARATOR;

		static const string STATISTICS_FILENAME;
		static const string STATISTICS_FILEPATH;
		static const string STATISTICS_FILE;
		static const string STATISTICS_SEPARATOR;

		static const string SERVER_ADDRESS;
		static const short SERVER_PORT;

		static const unsigned int MEET_SERVER_NODE_ID;

		static const unsigned int CLIENT_DELAY;
		static const unsigned int CLIENT_RANDOM_NUMBER_OF_MESSAGE_TYPES;
		static const unsigned int CLIENT_RANDOM_NUMBER_OF_CHANNELS;
		static const unsigned int CLIENT_RANDOM_NUMBER_OF_NEIGHBORS;
		static const unsigned int CLIENT_RANDOM_MAX_NUMBER_FOR_NEIGHBORS;

		static const unsigned int SAMPLE_NODES;
	};	
}

#endif
