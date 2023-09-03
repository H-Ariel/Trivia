#include "TriviaServer.h"
#include "SqliteDatabase.h"
#include "PacketSerializer.h"
#include "LoginRequestHandler.h"


TriviaServer::TriviaServer(uint16_t port)
	: _isRunning(false), _communicator(port)
{
	_database = DBG_NEW SqliteDatabase("trivia.db");
	_handlerFactory = DBG_NEW RequestHandlerFactory(_database);
}
TriviaServer::~TriviaServer()
{
	delete _handlerFactory;
	delete _database;
}

void TriviaServer::run()
{
	_isRunning = true;
	thread(&TriviaServer::startHandleRequests, this).detach();
	getCommands();
}

void TriviaServer::startHandleRequests()
{
	SOCKET clientSock;
	while (_isRunning)
	{
		cout << "Waiting for client connection request" << endl;

		clientSock = _communicator.accept();
		if (clientSock != INVALID_SOCKET)
		{
			thread(&TriviaServer::handleNewClient, this, clientSock).detach();
		}
	}
}

void TriviaServer::handleNewClient(SOCKET clientSock)
{
	cout << "start handle client. sock=" << clientSock << endl;

	RequestInfo reqInfo;
	RequestResult result;
	shared_ptr<IRequestHandler> handler = _handlerFactory->createLoginRequestHandler(clientSock);
	bool handleClient = true;

	try
	{
		while (handleClient && _isRunning)
		{
			reqInfo = _communicator.readFromSocket(clientSock);
			result = {};

			try
			{
				result = handler->handleRequest(reqInfo);

				if (result.changeHandler)
				{
					if (result.newHandler != nullptr)
					{
						handler = result.newHandler;
					}
					else
					{
						handleClient = false;
					}
				}
			}
			catch (const Exception& e)
			{
				result.response = ResponsePacketSerializer::serializeErrorResponse(e.what());
			}

			_communicator.sendToSocket(clientSock, result.response);
		}
	}
	catch (const Exception& e)
	{
		cout << "error in sock " << clientSock << ": " << e.what() << endl;
		_handlerFactory->getLoginManager().disconnectSocket(clientSock);
	}

	closesocket(clientSock);

	cout << "stop handle client. sock=" << clientSock << endl;
}

void TriviaServer::getCommands()
{
	static const string ExitCommands[] = { "exit", "quit", "q" };
	string cmd;
	while (_isRunning)
	{
		cin >> cmd;

		if (FIND_IN_ARRAY(ExitCommands, cmd))
		{
			_isRunning = false;
		}
		else
		{
			cout << "unknown command: " << cmd << endl;
		}
	}
}
