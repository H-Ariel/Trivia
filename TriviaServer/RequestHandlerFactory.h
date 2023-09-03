#pragma once

#include "LoginManager.h"
#include "RoomsManager.h"
#include "GamesManager.h"


class IRequestHandler;


class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* database);

	shared_ptr<IRequestHandler> createLoginRequestHandler(SOCKET sock); // connect socket to LoggedUser
	shared_ptr<IRequestHandler> createMenuRequestHandler(const LoggedUser& user);
	shared_ptr<IRequestHandler> createRoomMemberRequestHandler(const LoggedUser& user, unsigned int roomId);
	shared_ptr<IRequestHandler> createRoomAdminRequestHandler(const LoggedUser& user, unsigned int roomId);
	shared_ptr<IRequestHandler> createGameRequestHandler(const LoggedUser& user, unsigned int gameId); // the game ID is same to room ID

	LoginManager& getLoginManager() { return _loginManager; }
	RoomsManager& getRoomsManager() { return _roomsManager; }
	GamesManager& getGamesManager() { return _gamesManager; }


private:
	LoginManager _loginManager;
	RoomsManager _roomsManager;
	GamesManager _gamesManager;
};
