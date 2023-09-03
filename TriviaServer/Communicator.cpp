#include "Communicator.h"


Communicator::Communicator(uint16_t port)
	: serverSocket(INVALID_SOCKET)
{
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == INVALID_SOCKET)
		throw Exception(__FUNCTION__ " - socket");

	sockaddr_in sa = { };

	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw Exception(__FUNCTION__ " - bind");

	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw Exception(__FUNCTION__ " - listen");

	cout << "Listening on port " << port << endl;
}
Communicator::~Communicator()
{
	closesocket(serverSocket);
}

SOCKET Communicator::accept()
{
	return ::accept(serverSocket, NULL, NULL);
}

RequestInfo Communicator::readFromSocket(SOCKET sock)
{
	RequestInfo reqInfo;
	uint32_t msgLen = 0;
	
	// read message code
	int res = ::recv(sock, (char*)&reqInfo.msgCode, 1, 0);
	if (res != 1)
		throw Exception(__FUNCTION__ " - recv code (res=" + to_string(res) + ")");

	// read message length
	res = ::recv(sock, (char*)&msgLen, sizeof(msgLen), 0);
	if (res != sizeof(msgLen))
		throw Exception(__FUNCTION__ " - length (res=" + to_string(res) + ")");

	if (msgLen > 0)
	{
		// read message data
		reqInfo.msgData = string(msgLen, 0);
		res = ::recv(sock, (char*)reqInfo.msgData.data(), msgLen, 0);
		if (res != msgLen)
			throw Exception(__FUNCTION__ " - recv data (res=" + to_string(res) + ")");
	}

	return reqInfo;
}

void Communicator::sendToSocket(SOCKET sock, Buffer buffer)
{
	int res = ::send(sock, (const char*)buffer.data(), (int)buffer.size(), 0);
	if (res != (int)buffer.size())
		throw Exception(__FUNCTION__ " - send all (res=" + to_string(res) + ")");
}
