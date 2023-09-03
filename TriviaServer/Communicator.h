#pragma once

#include "GeneralLibrary.h"


class Communicator
{
public:
	Communicator(uint16_t port);
	~Communicator();

	SOCKET accept();
	RequestInfo readFromSocket(SOCKET sock);
	void sendToSocket(SOCKET sock, Buffer buffer);

private:
	SOCKET serverSocket;
};
