#pragma once

#include "GeneralLibrary.h"


class ISocket
{
public:
	virtual ~ISocket() = default;

	// for all sockets:

	virtual void send(const Buffer& msg) = 0;
	virtual void receive(char* buffer, int n) = 0;
	virtual Buffer receive(int n) = 0;
	virtual void close() = 0;


	// for servers:
	
	virtual void initServer(int port) = 0;
	virtual shared_ptr<ISocket> accept() = 0;
};
