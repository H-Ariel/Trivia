#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class RoomMemberRequestHandler : public IRequestHandler
{
public:
	RoomMemberRequestHandler(const LoggedUser& user, unsigned int roomId, RequestHandlerFactory& handlerFactory);
	RequestResult handleRequest(const RequestInfo& reqInfo) override;


protected:
	RequestResult handleGetRoomState();

	const LoggedUser _user;
	RoomsManager& _roomsManager;
	RequestHandlerFactory& _handlerFactory;
	const unsigned int _roomId;
};



class RoomAdminRequestHandler : public RoomMemberRequestHandler
{
public:
	RoomAdminRequestHandler(const LoggedUser& user, unsigned int roomId, RequestHandlerFactory& handlerFactory);
	RequestResult handleRequest(const RequestInfo& reqInfo) override;
};
