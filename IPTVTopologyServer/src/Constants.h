#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>
using namespace std;

class Constants{

public:
	static const string UNIVERSE_FILEPATH;
	static const string UNIVERSE_FILE;
	static const string UNIVERSE_FILENAME;
	static const string UNIVERSE_FILE_SEPARATORS;
};

const string Constants::UNIVERSE_FILEPATH = "src/app_data";
const string Constants::UNIVERSE_FILENAME = "uninet.txt";
const string Constants::UNIVERSE_FILE = Constants::UNIVERSE_FILEPATH + "/" + Constants::UNIVERSE_FILENAME;
const string Constants::UNIVERSE_FILE_SEPARATORS = " ";


#endif
