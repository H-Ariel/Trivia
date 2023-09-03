#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class GameRequestHandler : public IRequestHandler
{
public:
	GameRequestHandler(const LoggedUser& user, unsigned int gameId, RequestHandlerFactory& handlerFactory);
	RequestResult handleRequest(const RequestInfo& reqInfo) override;

private:
	const LoggedUser _user;
	RequestHandlerFactory& _handlerFactory;
	GamesManager& _gamesManager;
	const unsigned int _gameId;
};
