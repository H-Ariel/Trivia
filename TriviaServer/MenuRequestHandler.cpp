#include "MenuRequestHandler.h"
#include "PacketSerializer.h"
#include "RoomMemberRequestHandler.h"


MenuRequestHandler::MenuRequestHandler(const LoggedUser& user, RequestHandlerFactory& handlerFactory)
    : user(user), handlerFactory(handlerFactory), roomsManager(handlerFactory.getRoomsManager())
{
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	switch (reqInfo.msgCode)
	{
	case MessageCodes::Logout: {
		handlerFactory.getLoginManager().logout(user);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = nullptr;
	}	break;

	case MessageCodes::CreateRoom: {
		CreateRoomRequest req = RequestPacketDeserializer::deserializeCreateRoomRequest(reqInfo);
		unsigned int roomId = roomsManager.createRoom(user, RoomData(req.roomName, req.maxUsers, req.questionCount, req.answerTimeout));
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = handlerFactory.createRoomAdminRequestHandler(user, roomId);
	}	break;

	case MessageCodes::GetRooms: {
		result.response = ResponsePacketSerializer::serializeResponse(GetRoomsResponse(roomsManager.getRooms()));
		result.changeHandler = false;
	}	break;

	case MessageCodes::JoinRoom: {
		JoinRoomRequest req = RequestPacketDeserializer::deserializeJoinRoomRequest(reqInfo);
		roomsManager.addUserToRoom(user, req.roomId);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = handlerFactory.createRoomMemberRequestHandler(user, req.roomId);
	}	break;

	case MessageCodes::GetStatistics: {
		throw Exception("not impleted");
	}	break;

	default:
		throw Exception("irrelevant request");
	}

	return result;
}
