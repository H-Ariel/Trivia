#pragma once

#include "Communicator.h"
#include "IDatabase.h"
#include "RequestHandlerFactory.h"


class TriviaServer
{
public:
	TriviaServer(uint16_t port);
	~TriviaServer();

	void run();


private:
	void startHandleRequests();
	void handleNewClient(shared_ptr<ISocket>);
	void getCommands();


	Communicator _communicator;
	IDatabase* _database;
	RequestHandlerFactory* _handlerFactory;
	bool _isRunning;
};
