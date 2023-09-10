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
	return shared_ptr<IRequestHandler>(DBG_NEW LoginRequestHandler(*this, id));
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
