#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory, SOCKET sock);

	RequestResult handleRequest(const RequestInfo& reqInfo) override;
	
private:
	LoginManager& _loginManager;
	RequestHandlerFactory& _handlerFactory;
	SOCKET _sock;
};
