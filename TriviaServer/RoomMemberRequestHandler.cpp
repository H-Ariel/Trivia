#include "RoomMemberRequestHandler.h"
#include "PacketSerializer.h"
#include "MenuRequestHandler.h"
#include "GameRequestHandler.h"


RoomMemberRequestHandler::RoomMemberRequestHandler(const LoggedUser& user, unsigned int roomId, RequestHandlerFactory& handlerFactory)
	: _user(user), _handlerFactory(handlerFactory), _roomsManager(handlerFactory.getRoomsManager()), _roomId(roomId)
{
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	switch (reqInfo.msgCode)
	{
	case MessageCodes::GetRoomState:
		result = handleGetRoomState();
		break;

	case MessageCodes::LeaveRoom:
		_roomsManager.removeUserFromRoom(_user, _roomId);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = _handlerFactory.createMenuRequestHandler(_user);
		break;

	default:
		throw Exception("irrelevant request");
	}

	return result;
}

RequestResult RoomMemberRequestHandler::handleGetRoomState()
{
	RequestResult result;

	GetRoomStateResponse resp(_roomsManager.getRoomData(_roomId));
	resp.players = _roomsManager.getUsernames(_roomId);

	result.response = ResponsePacketSerializer::serializeResponse(resp);

	if (resp.isActive)
	{
		if (resp.hasGameStarted)
			result.newHandler = _handlerFactory.createGameRequestHandler(_user, _roomId);
		else
			result.changeHandler = false;
	}
	else
	{
		// leave room and back to menu
		_roomsManager.removeUserFromRoom(_user, _roomId);
		result.newHandler = _handlerFactory.createMenuRequestHandler(_user);
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
	case MessageCodes::GetRoomState:
		result = handleGetRoomState();
		break;

	case MessageCodes::LeaveRoom:
		_roomsManager.closeRoom(_roomId);
		_roomsManager.removeUserFromRoom(_user, _roomId);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = _handlerFactory.createMenuRequestHandler(_user);
		break;

	case MessageCodes::StartGame:
		_roomsManager.startGame(_roomId);
		_handlerFactory.getGamesManager().createGame(_roomId, _roomsManager.getUsers(_roomId), _roomsManager.getRoomData(_roomId).questionCount);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = _handlerFactory.createGameRequestHandler(_user, _roomId);
		break;

	default:
		throw Exception("irrelevant request");
	}

	return result;
}
