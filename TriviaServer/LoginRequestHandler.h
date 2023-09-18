#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory* handlerFactory, void* _key);

	RequestResult handleRequest(const RequestInfo&) override;
	
private:
	RequestResult login(const RequestInfo&);
	RequestResult signup(const RequestInfo&);

	RequestHandlerFactory* _handlerFactory;
	void* _key; // special key for the user
};
