#include "Logger.h"
#include "Server.h"

using namespace std;

int main()
{
	try
	{
		Server s;
		s.Start();

		system("PAUSE");
	}
	catch (exception& e)
	{
		IPTVCommon::Logger::Current().LogError(e.what());
	}
}