#include "WSAInitializer.h"
#include "GeneralLibrary.h"


WSAInitializer::WSAInitializer()
{
	WSADATA wsa_data = {};
	WSAStartup(MAKEWORD(2, 2), &wsa_data);
}

WSAInitializer::~WSAInitializer()
{
	WSACleanup();
}
