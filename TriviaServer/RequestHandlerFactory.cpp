#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"


RequestHandlerFactory::RequestHandlerFactory(IDatabase* database)
	: loginManager(database), roomsManager(database), gamesManager(database)
{
}

shared_ptr<IRequestHandler> RequestHandlerFactory::createLoginRequestHandler(SOCKET sock)
{
	return shared_ptr<IRequestHandler>(DBG_NEW LoginRequestHandler(*this, sock));
}

shared_ptr<IRequestHandler> RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
	return shared_ptr<IRequestHandler>(DBG_NEW MenuRequestHandler(user, *this));
}

shared_ptr<IRequestHandler> RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, unsigned int roomId)
{
	return shared_ptr<IRequestHandler>(DBG_NEW RoomMemberRequestHandler(user, roomId, *this));
}

shared_ptr<IRequestHandler> RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, unsigned int roomId)
{
	return shared_ptr<IRequestHandler>(DBG_NEW RoomAdminRequestHandler(user, roomId, *this));
}

shared_ptr<IRequestHandler> RequestHandlerFactory::createGameRequestHandler(const LoggedUser& user, unsigned int gameId)
{
	return shared_ptr<IRequestHandler>(DBG_NEW GameRequestHandler(user, gameId, *this));
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return loginManager;
}

RoomsManager& RequestHandlerFactory::getRoomsManager()
{
	return roomsManager;
}

GamesManager& RequestHandlerFactory::getGamesManager()
{
	return gamesManager;
}
