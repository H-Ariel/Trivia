#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory* handlerFactory, int id);

	RequestResult handleRequest(const RequestInfo&) override;
	
private:
	RequestResult login(const RequestInfo&);
	RequestResult signup(const RequestInfo&);

	RequestHandlerFactory* _handlerFactory;
	int _id;
};
