#include "MenuRequestHandler.h"
#include "PacketSerializer.h"
#include "RoomMemberRequestHandler.h"


MenuRequestHandler::MenuRequestHandler(const LoggedUser& user, RequestHandlerFactory& handlerFactory)
	: _user(user), _handlerFactory(handlerFactory), _roomsManager(handlerFactory.getRoomsManager()), _statisticsManager(handlerFactory.getStatisticsManager())
{
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	switch (reqInfo.msgCode)
	{
	case MessageCodes::Logout:
		_handlerFactory.getLoginManager().logout(_user);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = nullptr;
		break;

	case MessageCodes::CreateRoom:
		result.newHandler = _handlerFactory.createRoomAdminRequestHandler(_user, _roomsManager.createRoom(_user,
			RoomData(RequestPacketDeserializer::deserializeCreateRoomRequest(reqInfo))));
		result.response = ResponsePacketSerializer::serializeOkResponse();
		break;

	case MessageCodes::GetRooms:
		result.response = ResponsePacketSerializer::serializeResponse(GetRoomsResponse(_roomsManager.getRooms()));
		result.changeHandler = false;
		break;

	case MessageCodes::JoinRoom: {
		JoinRoomRequest req = RequestPacketDeserializer::deserializeJoinRoomRequest(reqInfo);
		_roomsManager.addUserToRoom(_user, req.roomId);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = _handlerFactory.createRoomMemberRequestHandler(_user, req.roomId);
	}	break;

	case MessageCodes::GetStatistics: {
	//	throw Exception("not impleted");
		result.response = ResponsePacketSerializer::serializeResponse(
			GetStatisticsResponse(_statisticsManager.getHighScore(), _statisticsManager.getUserStatistics(_user.getUsername()))
		);
		result.changeHandler = false;
	}	break;

	default:
		throw Exception("irrelevant request");
	}

	return result;
}
