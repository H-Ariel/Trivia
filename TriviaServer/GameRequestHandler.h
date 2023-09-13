#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class GameRequestHandler : public IRequestHandler
{
public:
	GameRequestHandler(RequestHandlerFactory* handlerFactory, const LoggedUser& user, unsigned int gameId);
	RequestResult handleRequest(const RequestInfo& reqInfo) override;

private:
	RequestResult getQuestion();
	RequestResult submitAnswer(const RequestInfo&);
	RequestResult getGameResults();
	RequestResult leaveGame();

	const LoggedUser _user;
	RequestHandlerFactory* _handlerFactory;
	const unsigned int _gameId;
};
