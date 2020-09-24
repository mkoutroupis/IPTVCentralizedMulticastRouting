#include "UniverseParserMode.h"
#include "IUniverseParser.h"
#include "UniverseFileParser.h"

IPTVCommon::UniverseFileParser::UniverseFileParser() :
IUniverseParser()
{
	_separators = new boost_generic::char_separator<char>(separator.c_str());
	_current_num_line = 0;
	_total_num_lines = 0;
}

IPTVCommon::UniverseFileParser::UniverseFileParser(const string filename, const string separator, const string format) :
IUniverseParser(separator, format), _filename(filename)
{
	_separators = new boost_generic::char_separator<char>(separator.c_str());
	_current_num_line = 0;
	_total_num_lines = 0;

	try
	{
		int _neighbor_startIndex = format.find_last_of("{");
		int _neighbor_endIndex = format.find_first_of("}", _neighbor_startIndex);
		_neighbor_prefix = format.substr(0, _neighbor_startIndex);
		_neighbor_suffix = format.substr(_neighbor_endIndex + 1);

		IPTVCommon::Logger::Current().LogDebug("UniverseFileParser instantiated successfully.");
	}
	catch (exception& e){
		string exceptionMessage(e.what());
		string errorMessage = "Malformed neighbor format. Exception: " + exceptionMessage;
		cerr << errorMessage << endl;
		throw errorMessage;
	}
}

IPTVCommon::UniverseFileParser::~UniverseFileParser()
{
	delete _separators;
	_separators = nullptr;

	IPTVCommon::Logger::Current().LogDebug("UniverseFileParser deleted successfully.");
}


void IPTVCommon::UniverseFileParser::_Parse()
{
	bool isTestMode = _mode == Test;
	unsigned int counter = 0;

	ifstream in(_filename);

	if (!in.is_open()) throw "File cannot be opened!";

	string line;

	while (getline(in, line))
	{
		counter++;

		Tokenizer tok(line, (*_separators));

		Tokenizer::iterator it = tok.begin();
		if (it == tok.end())  continue;

		unsigned int key = 0;
		vector<unsigned int> neighbors;

		try {
			key = boost::lexical_cast<unsigned int>(*it);
		}
		catch (boost::bad_lexical_cast const&) {
			string errorMessage = "Key value is invalid. Try to convert string " + boost::lexical_cast<string>(*it) + " to int. At line: " + boost::lexical_cast<string>(counter);
			cerr << errorMessage << endl;
			throw errorMessage;
		}

		++it;

		for (; it != tok.end(); ++it){
			try {
				string token = *it;

				int startIndex = it->find_last_of(_neighbor_prefix);
				int lastIndex = it->find_first_of(_neighbor_suffix, startIndex);

				if (lastIndex == -1 || startIndex == -1) throw "Bad format. Token " + boost::lexical_cast<string>(*it) + " At line: " + boost::lexical_cast<string>(counter);

				string neighbor_value = it->substr(startIndex + _neighbor_prefix.length(), lastIndex - startIndex - _neighbor_prefix.length());

				unsigned int neighbor_id = boost::lexical_cast<unsigned int>(neighbor_value);
				if (!isTestMode) neighbors.push_back(neighbor_id);
			}
			catch (boost::bad_lexical_cast const&) {
				string errorMessage = "Value is invalid. Try to convert string " + boost::lexical_cast<string>(*it) + " to int. At line: " + boost::lexical_cast<string>(counter);
				cerr << errorMessage << endl;
				throw errorMessage;
			}
			catch (exception& e){
				string errorMessage = e.what();
				cerr << errorMessage << endl;
				throw errorMessage;
			}
		}

		if (!isTestMode) m_universe.insert(make_pair(key, neighbors));
	}

	if (!isTestMode)
	{
		_total_num_lines = counter;
	}
	_current_num_line = counter;

	if (_total_num_lines != _current_num_line && !isTestMode)
	{
		string errorMessage = "Number of lines is not consistent with validate. Found: " + boost::lexical_cast<string>(_total_num_lines)+" on validate. Now: " + boost::lexical_cast<string>(_current_num_line);
		cerr << errorMessage << endl;
		throw errorMessage;
	}
}

void IPTVCommon::UniverseFileParser::Parse(UniverseParserMode mode) {
	_mode = mode;

	_Parse();
}

void IPTVCommon::UniverseFileParser::Validate(UniverseParserMode mode) {
	_mode = mode;

	boost_filesystem::path p(_filename);

	if (!boost_filesystem::exists(p))
		throw "File does not exist!";
	if (!boost_filesystem::is_regular_file(p))
		throw "File is corrupted!";

	_Parse();
}
