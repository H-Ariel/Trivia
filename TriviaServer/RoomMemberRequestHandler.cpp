#include "RoomMemberRequestHandler.h"
#include "PacketSerializer.h"
#include "MenuRequestHandler.h"
#include "GameRequestHandler.h"


RoomMemberRequestHandler::RoomMemberRequestHandler(const LoggedUser& user, unsigned int roomId, RequestHandlerFactory& handlerFactory)
	: user(user), handlerFactory(handlerFactory), roomsManager(handlerFactory.getRoomsManager()), roomId(roomId)
{
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	switch (reqInfo.msgCode)
	{
	case MessageCodes::GetRoomState: {
		result = handleGetRoomState();
	}	break;

	case MessageCodes::LeaveRoom: {
		roomsManager.removeUserFromRoom(user, roomId);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = handlerFactory.createMenuRequestHandler(user);
	}	break;

	default:
		throw Exception("irrelevant request");
	}

	return result;
}

RequestResult RoomMemberRequestHandler::handleGetRoomState()
{
	RequestResult result;

	GetRoomStateResponse resp(roomsManager.getRoomData(roomId));
	resp.players = roomsManager.getUsernames(roomId);

	result.response = ResponsePacketSerializer::serializeResponse(resp);

	if (resp.isActive)
	{
		if (resp.hasGameStarted)
			result.newHandler = handlerFactory.createGameRequestHandler(user, roomId);
		else
			result.changeHandler = false;
	}
	else
	{
		// leave room and back to menu
		roomsManager.removeUserFromRoom(user, roomId);
		result.newHandler = handlerFactory.createMenuRequestHandler(user);
	}


	return result;
}




RoomAdminRequestHandler::RoomAdminRequestHandler(const LoggedUser& user, unsigned int roomId, RequestHandlerFactory& handlerFactory)
	: RoomMemberRequestHandler(user, roomId, handlerFactory)
{
}


RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	switch (reqInfo.msgCode)
	{
	case MessageCodes::GetRoomState: {
		result = handleGetRoomState();
	}	break;

	case MessageCodes::LeaveRoom: {
		roomsManager.closeRoom(roomId);
		roomsManager.removeUserFromRoom(user, roomId);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = handlerFactory.createMenuRequestHandler(user);
	}	break;

	case MessageCodes::StartGame: {
		roomsManager.startGame(roomId);
		handlerFactory.getGamesManager().createGame(roomId, roomsManager.getUsers(roomId), roomsManager.getRoomData(roomId).questionCount);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = handlerFactory.createGameRequestHandler(user, roomId);
	}	break;

	default:
		throw Exception("irrelevant request");
	}

	return result;
}
