#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(RequestHandlerFactory* handlerFactory, const LoggedUser& user);

	RequestResult handleRequest(const RequestInfo&) override;

private:
	RequestResult logout();
	RequestResult createRoom(const RequestInfo&);
	RequestResult getRooms();
	RequestResult joinRoom(const RequestInfo&);
	RequestResult getStatistics();

	const LoggedUser _user;
	RequestHandlerFactory* _handlerFactory;
};
