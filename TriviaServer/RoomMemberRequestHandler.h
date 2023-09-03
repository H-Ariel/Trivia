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

	const LoggedUser user;
	RoomsManager& roomsManager;
	RequestHandlerFactory& handlerFactory;
	const unsigned int roomId;
};



class RoomAdminRequestHandler : public RoomMemberRequestHandler
{
public:
	RoomAdminRequestHandler(const LoggedUser& user, unsigned int roomId, RequestHandlerFactory& handlerFactory);
	RequestResult handleRequest(const RequestInfo& reqInfo) override;
};
