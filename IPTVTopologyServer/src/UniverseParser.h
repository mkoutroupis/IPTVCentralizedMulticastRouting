/*
 * UniverseParser.h
 *
 *  Created on: Aug 6, 2014
 *      Author: mike
 */

#ifndef UNIVERSEPARSER_H_
#define UNIVERSEPARSER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/tokenizer.hpp>
#include "Constants.h"

namespace boost_filesystem = boost::filesystem;
namespace boost_generic = boost;

using namespace std;

class UniverseParser {
	string _filename;
	boost_generic::char_separator<char>* _separators;

	void CheckFile() {
		boost_filesystem::path p(_filename); // p reads clearer than argv[1] in the following code

		if (!boost_filesystem::exists(p))
			throw "File does not exist!";
		if (!boost_filesystem::is_regular_file(p))
			throw "File is corrupted!";
	}
public:
	UniverseParser(const string filename, const string separator):
		_filename(filename)
	{
		_separators = new boost_generic::char_separator<char>(separator.c_str());
	}

	void Parse() {
		ifstream in(_filename);

	     if (!in.is_open()) throw "File cannot be opened!";

			vector<string> vec;
		    string line;

		    typedef boost_generic::tokenizer<boost_generic::char_separator<char> >   tokenizer;

		    tokenizer tok(line, (*_separators));

		    while (getline(in,line))
		    {


		    	vec.assign(tok.begin(),tok.end());

		        if (vec.size() < 3) continue;

		        copy(vec.begin(), vec.end(),
		             ostream_iterator<string>(cout, "|"));

		        cout << "\n----------------------" << endl;
		    }
	}

	void Convert2Graph() {

	}
};

#endif /* UNIVERSEPARSER_H_ */
