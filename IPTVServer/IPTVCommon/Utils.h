#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>

namespace IPTVCommon {

	using namespace std;

	class Utils
	{
	public:

		template<typename T>
		static string Convert2String(vector<T> vec){
			ostringstream oss;

			if (!vec.empty())
			{
				// Convert all but the last element to avoid a trailing ","
				copy(vec.begin(), vec.end() - 1, ostream_iterator<T>(oss, ","));

				// Now add the last element with no delimiter
				oss << vec.back();
			}

			return oss.str();
		}

		template<typename T>
		static vector<T> Convert2Vector(string s){
			vector<T> tokens;
			
			istringstream iss(s);
			string token;

			while (getline(iss, token, ',')) {
				tokens.push_back(boost::lexical_cast<T>(token));
			}

			return tokens;
		}

		template<typename T>
		static vector<set<T>> GetNFromSet(set<T> v, unsigned int returnSize, unsigned int setSize){

			vector<set<unsigned int>> r;

			srand(time(NULL));

			for (unsigned int i = 0; i < returnSize; i++)
			{
				set<T> s;
				while (s.size() < setSize)
				{
					int selected_index = rand() % v.size();

					T selected_element = *next(v.begin(), selected_index);

					s.insert(selected_element);
				}

				r.push_back(s);
			}

			return r;
		}
		
		template<typename T>
		static void PrintVector(vector<T> v){

			for (vector<T>::iterator it = v.begin(); it != v->end(); it++){
				cout << *it << " ";
			}
			cout << endl;
		}

		static vector<char>  ConvertInt2Bytes(unsigned int integer)
		{
			vector<char>  arrayOfByte;
			arrayOfByte.push_back((int)((integer >> 24) & 0xFF));
			arrayOfByte.push_back((int)((integer >> 16) & 0xFF));
			arrayOfByte.push_back((int)((integer >> 8) & 0XFF));
			arrayOfByte.push_back((int)((integer & 0XFF)));
			return arrayOfByte;
		}

		static unsigned int ConvertBytes2Int(char* byteArray)
		{
			return ((byteArray[0] << 24)
				+ (byteArray[1] << 16)
				+ (byteArray[2] << 8)
				+ (byteArray[3]));
		}


	};
}
#endif

