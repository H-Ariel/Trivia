#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory, int id);

	RequestResult handleRequest(const RequestInfo& reqInfo) override;
	
private:
	LoginManager& _loginManager;
	RequestHandlerFactory& _handlerFactory;
	int _id;
};
