#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class RoomMemberRequestHandler : public IRequestHandler
{
public:
	RoomMemberRequestHandler(RequestHandlerFactory* handlerFactory, const LoggedUser& user, int roomId);
	RequestResult handleRequest(const RequestInfo&) override;

protected:
	RequestResult getRoomState();
	virtual RequestResult leaveRoom();

	const LoggedUser _user;
	RequestHandlerFactory* _handlerFactory;
	const int _roomId;
};



class RoomAdminRequestHandler : public RoomMemberRequestHandler
{
public:
	RoomAdminRequestHandler(RequestHandlerFactory* handlerFactory, const LoggedUser& user, int roomId);
	RequestResult handleRequest(const RequestInfo&) override;

private:
	RequestResult leaveRoom() override;
	RequestResult startGame();
};
