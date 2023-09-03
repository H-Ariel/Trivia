#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(const LoggedUser& user, RequestHandlerFactory& handlerFactory);

	RequestResult handleRequest(const RequestInfo& reqInfo) override;

private:
	const LoggedUser _user;
	RoomsManager& _roomsManager;
	//StatisticsManager& statisticsManager; TODO
	RequestHandlerFactory& _handlerFactory;
};
