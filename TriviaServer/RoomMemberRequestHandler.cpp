#include "RoomMemberRequestHandler.h"
#include "PacketSerializer.h"


RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory* handlerFactory, const LoggedUser& user, int roomId)
	: _user(user), _handlerFactory(handlerFactory), _roomId(roomId)
{
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	switch (reqInfo.msgCode)
	{
	case MessageCodes::GetRoomState: return getRoomState();
	case MessageCodes::LeaveRoom: return leaveRoom();
	}
	throw Exception("irrelevant request");
}

RequestResult RoomMemberRequestHandler::getRoomState()
{
	RequestResult result;

	RoomsManager& roomsManager = _handlerFactory->getRoomsManager();

	GetRoomStateResponse resp(roomsManager.getRoomData(_roomId));
	resp.players = roomsManager.getUsernames(_roomId);

	result.response = ResponsePacketSerializer::serializeResponse(resp);

	if (resp.isActive)
	{
		if (resp.hasGameStarted)
			result.newHandler = _handlerFactory->createGameRequestHandler(_user, _roomId);
		else
			result.changeHandler = false;
	}
	else
	{
		// leave room and back to menu
		roomsManager.removeUserFromRoom(_user, _roomId);
		result.newHandler = _handlerFactory->createMenuRequestHandler(_user);
	}

	return result;
}
RequestResult RoomMemberRequestHandler::leaveRoom()
{
	_handlerFactory->getRoomsManager().removeUserFromRoom(_user, _roomId);
	return RequestResult(ResponsePacketSerializer::serializeOkResponse(),
		_handlerFactory->createMenuRequestHandler(_user));
}


RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory* handlerFactory, const LoggedUser& user, int roomId)
	: RoomMemberRequestHandler(handlerFactory, user, roomId)
{
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	switch (reqInfo.msgCode)
	{
	case MessageCodes::GetRoomState: return getRoomState();
	case MessageCodes::LeaveRoom: return leaveRoom();
	case MessageCodes::StartGame: return startGame();
	}
	throw Exception("irrelevant request");
}

RequestResult RoomAdminRequestHandler::leaveRoom()
{
 	RoomsManager& roomsManager = _handlerFactory->getRoomsManager();

	roomsManager.closeRoom(_roomId);
	roomsManager.removeUserFromRoom(_user, _roomId);
	
	return RequestResult(ResponsePacketSerializer::serializeOkResponse(),
		_handlerFactory->createMenuRequestHandler(_user));
}
RequestResult RoomAdminRequestHandler::startGame()
{
	RoomsManager& roomsManager = _handlerFactory->getRoomsManager();
	roomsManager.startGame(_roomId);
	_handlerFactory->getGamesManager().createGame(roomsManager.getRoomData(_roomId), roomsManager.getUsers(_roomId));
	return RequestResult(ResponsePacketSerializer::serializeOkResponse(),
		_handlerFactory->createGameRequestHandler(_user, _roomId));
}
