#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include "ISocket.h"

#pragma comment (lib, "ws2_32.lib")


class WindowsSocket : public ISocket
{
public:
	WindowsSocket(SOCKET sock = INVALID_SOCKET);
	~WindowsSocket() override;

	void send(const Buffer& msg) override;
	void receive(char* buffer, int n) override;
	Buffer receive(int n) override;
	void close() override;

	void initServer(int port) override;
	shared_ptr<ISocket> accept() override;

protected:
	SOCKET _sock;
	static int _sockCount;
};
