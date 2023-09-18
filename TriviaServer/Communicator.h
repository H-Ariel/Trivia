#pragma once

#include "ISocket.h"


class Communicator
{
public:
	Communicator(uint16_t port);
	~Communicator();

	shared_ptr<ISocket> accept();
	RequestInfo readFromSocket(shared_ptr<ISocket> sock);
	void sendToSocket(shared_ptr<ISocket> sock, RequestInfo reqInfo);

private:
	shared_ptr<ISocket> _serverSocket;
};
