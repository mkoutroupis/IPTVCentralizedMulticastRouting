#include "Server.h"

Server::Server()
{
	try
	{
		auto uParser = new UniverseFileParser(Constants::UNIVERSE_FILE, Constants::UNIVERSE_NEIGHBOR_SEPARATOR, Constants::UNIVERSE_NEIGHBOR_FORMAT);
		_u_parser = *uParser;
		auto uHolder = new UniverseHolder(_u_parser);
		auto subHolder = new SubscriptionHolder();
		_u_holder = *uHolder;
		_sub_holder = *subHolder;
		//_m_holder = new MessageHolder(_io_service, Constants::SERVER_PORT,_u_parser , _u_holder,_sub_holder);
		_m_holder = new MessageHolderForStats(_io_service, Constants::SERVER_PORT, _u_parser, _u_holder, _sub_holder, Constants::SAMPLE_NODES);
		/*auto mHolder = new MessageHolder(_io_service, Constants::SERVER_PORT);
		_m_holder = *mHolder;*/

		IPTVCommon::Logger::Current().LogDebug("Server instantiated successfully.");
	}
	catch (exception& e)
	{
		string exceptionMessage(e.what());
		string errorMessage = "Server error during instantiation. Exception: " + exceptionMessage;
		cerr << errorMessage << endl;
		throw errorMessage;
	}
}


Server::~Server()
{
	delete _m_holder;
	/*delete _parser;
	delete _holder ;
	delete _sub_holder;*/
	IPTVCommon::Logger::Current().LogDebug("Server deleted successfully.");
}

void Server::Start()
{
	_io_service.run();
	IPTVCommon::Logger::Current().LogDebug("Server started.");
}

void Server::Stop()
{
	_io_service.stop();
	IPTVCommon::Logger::Current().LogDebug("Server stopped.");
}

void Server::Reset()
{
	IPTVCommon::Logger::Current().LogDebug("Server was reseted.");
}

