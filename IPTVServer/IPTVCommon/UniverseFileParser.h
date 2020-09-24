#ifndef UNIVERSEFILEPARSER_H_
#define UNIVERSEFILEPARSER_H_

#include "IUniverseParser.h"
#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "Logger.h"

using namespace std;

namespace boost_filesystem = boost::filesystem;
namespace boost_generic = boost;

namespace IPTVCommon {

	class UniverseFileParser : public IUniverseParser, public enable_shared_from_this < UniverseFileParser > {
		typedef boost_generic::tokenizer<boost_generic::char_separator<char>> Tokenizer;

		string _filename;
		string _neighbor_prefix, _neighbor_suffix;
		boost_generic::char_separator<char>* _separators = nullptr;
		unsigned int _current_num_line;
		unsigned int _total_num_lines;

		void _Parse();

	public:
		shared_ptr<UniverseFileParser> getptr() {
			return shared_from_this();
		}

		UniverseFileParser();
		UniverseFileParser(const string filename, const string separator, const string format);
		~UniverseFileParser();

		void Validate(UniverseParserMode mode = Normal);
		void Parse(UniverseParserMode mode = Normal);
	};
}

#endif