#include "WindowsSocket.h"


int WindowsSocket::_sockCount = 0;


WindowsSocket::WindowsSocket(SOCKET sock)
	: _sock(sock)
{
	if (_sockCount == 0)
	{
		WSADATA wsa_data = {};
		WSAStartup(MAKEWORD(2, 2), &wsa_data);
	}

	_sockCount++;
}

WindowsSocket::~WindowsSocket()
{
	close();

	_sockCount--;
	if (_sockCount == 0)
	{
		WSACleanup();
	}
}

void WindowsSocket::send(const Buffer& msg)
{
	int res = ::send(_sock, (const char*)msg.data(), (int)msg.size(), 0);
	if (res != (int)msg.size())
		throw Exception(__FUNCTION__ ": res=" + to_string(res));
}

void WindowsSocket::receive(char* buffer, int n)
{
	if (n <= 0) return;

	int res = ::recv(_sock, buffer, n, 0);
	if (res != n)
		throw Exception(__FUNCTION__ ": res=" + to_string(res));
}

Buffer WindowsSocket::receive(int n)
{
	if (n <= 0) return Buffer();

	char* tmp = DBG_NEW char[n];
	receive(tmp, n);
	Buffer buf((uint8_t*)tmp, (uint8_t*)tmp + n);
	delete[] tmp;
	return buf;
}

void WindowsSocket::close()
{
	if (_sock != INVALID_SOCKET)
	{
		closesocket(_sock);
		_sock = INVALID_SOCKET;
	}
}

void WindowsSocket::initServer(int port)
{
	if (_sock != INVALID_SOCKET) close();

	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_sock == INVALID_SOCKET)
		throw Exception(__FUNCTION__ " - socket");

	sockaddr_in sa = {};

	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	if (bind(_sock, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		closesocket(_sock);
		_sock = INVALID_SOCKET;
		throw Exception(__FUNCTION__ " - bind");
	}

	if (listen(_sock, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(_sock);
		_sock = INVALID_SOCKET;
		throw Exception(__FUNCTION__ " - listen");
	}
}

shared_ptr<ISocket> WindowsSocket::accept()
{
	SOCKET newSock = ::accept(_sock, NULL, NULL);
	if (newSock == INVALID_SOCKET)
		return shared_ptr<ISocket>(nullptr);
	return shared_ptr<ISocket>(DBG_NEW WindowsSocket(newSock));
}
