#ifndef IUNIVERSEPARSER_H_
#define IUNIVERSEPARSER_H_

#include "UniverseParserMode.h"
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace boost_generic = boost;

namespace IPTVCommon {
	class IUniverseParser
	{
	protected:
		UniverseParserMode _mode;
	public:
		string format = "";
		string separator = "";
		map<unsigned int, vector<unsigned int>> m_universe;

		IUniverseParser(){};
		IUniverseParser(const string separator, const string format) : separator(separator), format(format){};
		~IUniverseParser(){};

		virtual void Validate(UniverseParserMode mode = Normal) = 0;
		virtual void Parse(UniverseParserMode mode = Normal) = 0;
		void Print()
		{
			for (map<unsigned int, vector<unsigned int>>::iterator i = m_universe.begin(); i != m_universe.end(); ++i)
			{
				string output_line = "";
				output_line += boost::lexical_cast<string>(i->first) + "-> ";

				vector<unsigned int> neighbors = i->second;
				for (vector<unsigned int>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					output_line += boost::lexical_cast<string>(*it) + " ";
				}
				output_line += "\n";

				cout << output_line << "\n";
			}
		}
		set<unsigned int> GetUniverse(){
			set<unsigned int> universe;

			for (map<unsigned int, vector<unsigned int>>::iterator i = m_universe.begin(); i != m_universe.end(); ++i)
			{
				universe.insert(i->first);

				vector<unsigned int> neighbors = i->second;
				for (vector<unsigned int>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					universe.insert(*it);
				}
			}

			return universe;
		}
		UniverseParserMode GetMode(){
			return _mode;
		}
	};
}

#endif