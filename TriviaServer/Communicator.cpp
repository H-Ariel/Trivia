#include "WindowsSocket.h"
#include "Communicator.h"


Communicator::Communicator(uint16_t port)
	: _serverSocket(DBG_NEW WindowsSocket)
{
	_serverSocket->initServer(port);

	cout << "Listening on port " << port << endl;
}
Communicator::~Communicator()
{
	_serverSocket->close();
}

shared_ptr<ISocket> Communicator::accept()
{
	return _serverSocket->accept();
}

RequestInfo Communicator::readFromSocket(shared_ptr<ISocket> sock)
{
	RequestInfo reqInfo;
	uint32_t msgLen = 0;

	sock->receive((char*)&reqInfo.msgCode, 1); // read message code
	sock->receive((char*)&msgLen, sizeof(msgLen)); // read message length
	reqInfo.msgData = sock->receive(msgLen); // read message data

	return reqInfo;
}
void Communicator::sendToSocket(shared_ptr<ISocket> sock, RequestInfo reqInfo)
{
	Buffer buffer;
	uint32_t len = (uint32_t)reqInfo.msgData.size();

	buffer.push_back(reqInfo.msgCode);
	buffer.insert(buffer.end(), (uint8_t*)(&len), (uint8_t*)(&len + 1));
	buffer.insert(buffer.end(), reqInfo.msgData.begin(), reqInfo.msgData.end());

	sock->send(buffer);
}
