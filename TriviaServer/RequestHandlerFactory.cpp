#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"


RequestHandlerFactory::RequestHandlerFactory(IDatabase* database)
	: _loginManager(database), _statisticsManager(database), _roomsManager(database), _gamesManager(database)
{
}

shared_ptr<IRequestHandler> RequestHandlerFactory::createLoginRequestHandler(int id)
{
	return shared_ptr<IRequestHandler>(DBG_NEW LoginRequestHandler(this, id));
}

shared_ptr<IRequestHandler> RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
	return shared_ptr<IRequestHandler>(DBG_NEW MenuRequestHandler(this, user));
}

shared_ptr<IRequestHandler> RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, int roomId)
{
	return shared_ptr<IRequestHandler>(DBG_NEW RoomMemberRequestHandler(this, user, roomId));
}

shared_ptr<IRequestHandler> RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, int roomId)
{
	return shared_ptr<IRequestHandler>(DBG_NEW RoomAdminRequestHandler(this, user, roomId));
}

shared_ptr<IRequestHandler> RequestHandlerFactory::createGameRequestHandler(const LoggedUser& user, int gameId)
{
	return shared_ptr<IRequestHandler>(DBG_NEW GameRequestHandler(this, user, gameId));
}
