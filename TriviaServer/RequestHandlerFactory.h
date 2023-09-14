#pragma once

#include "LoginManager.h"
#include "StatisticsManager.h"
#include "RoomsManager.h"
#include "GamesManager.h"


class IRequestHandler;


class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* database);

	shared_ptr<IRequestHandler> createLoginRequestHandler(int id); // connect socket (id) to LoggedUser
	shared_ptr<IRequestHandler> createMenuRequestHandler(const LoggedUser& user);
	shared_ptr<IRequestHandler> createRoomMemberRequestHandler(const LoggedUser& user, int roomId);
	shared_ptr<IRequestHandler> createRoomAdminRequestHandler(const LoggedUser& user, int roomId);
	shared_ptr<IRequestHandler> createGameRequestHandler(const LoggedUser& user, int gameId); // the game ID is same to room ID

	LoginManager& getLoginManager() { return _loginManager; }
	StatisticsManager& getStatisticsManager() { return _statisticsManager; }
	RoomsManager& getRoomsManager() { return _roomsManager; }
	GamesManager& getGamesManager() { return _gamesManager; }


private:
	LoginManager _loginManager;
	StatisticsManager _statisticsManager;
	RoomsManager _roomsManager;
	GamesManager _gamesManager;
};
