#include "Communicator.h"


Communicator::Communicator(uint16_t port)
	: _serverSocket(INVALID_SOCKET)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_serverSocket == INVALID_SOCKET)
		throw Exception(__FUNCTION__ " - socket");

	sockaddr_in sa = {};

	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	if (bind(_serverSocket, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		closesocket(_serverSocket);
		throw Exception(__FUNCTION__ " - bind");
	}

	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(_serverSocket);
		throw Exception(__FUNCTION__ " - listen");
	}

	cout << "Listening on port " << port << endl;
}
Communicator::~Communicator()
{
	closesocket(_serverSocket);
}

SOCKET Communicator::accept()
{
	return ::accept(_serverSocket, NULL, NULL);
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
		throw Exception(__FUNCTION__ " - recv length (res=" + to_string(res) + ")");

	if (msgLen > 0)
	{
		// read message data
		reqInfo.msgData = Buffer(msgLen, 0);
		res = ::recv(sock, (char*)reqInfo.msgData.data(), msgLen, 0);
		if (res != msgLen)
			throw Exception(__FUNCTION__ " - recv data (res=" + to_string(res) + ")");
	}

	return reqInfo;
}

void Communicator::sendToSocket(SOCKET sock, RequestInfo reqInfo)
{
	Buffer buffer;
	uint32_t len = (uint32_t)reqInfo.msgData.size();

	buffer.push_back(reqInfo.msgCode);
	buffer.insert(buffer.end(), (uint8_t*)(&len), (uint8_t*)(&len + 1));
	buffer.insert(buffer.end(), reqInfo.msgData.begin(), reqInfo.msgData.end());

	int res = ::send(sock, (const char*)buffer.data(), (int)buffer.size(), 0);
	if (res != (int)buffer.size())
		throw Exception(__FUNCTION__ " - send all (res=" + to_string(res) + ")");
}
