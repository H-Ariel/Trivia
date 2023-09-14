#include "MenuRequestHandler.h"
#include "PacketSerializer.h"


MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory* handlerFactory, const LoggedUser& user)
	: _user(user), _handlerFactory(handlerFactory)
{
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	switch (reqInfo.msgCode)
	{
	case MessageCodes::Logout: return logout();
	case MessageCodes::CreateRoom: return createRoom(reqInfo);
	case MessageCodes::GetRooms: return getRooms();
	case MessageCodes::JoinRoom: return joinRoom(reqInfo);
	case MessageCodes::GetStatistics: return getStatistics();
	}
	throw Exception("irrelevant request");
}

RequestResult MenuRequestHandler::logout()
{
	_handlerFactory->getLoginManager().logout(_user);
	return RequestResult(ResponsePacketSerializer::serializeOkResponse(), nullptr);
}
RequestResult MenuRequestHandler::createRoom(const RequestInfo& reqInfo)
{
	return RequestResult(ResponsePacketSerializer::serializeOkResponse(),
		_handlerFactory->createRoomAdminRequestHandler(_user, _handlerFactory->getRoomsManager().createRoom(_user,
			RoomData(RequestPacketDeserializer::deserializeCreateRoomRequest(reqInfo)))));
}
RequestResult MenuRequestHandler::getRooms()
{
	return RequestResult(ResponsePacketSerializer::serializeResponse(GetRoomsResponse(_handlerFactory->getRoomsManager().getRooms())));
}
RequestResult MenuRequestHandler::joinRoom(const RequestInfo& reqInfo)
{
	int id = RequestPacketDeserializer::deserializeJoinRoomRequest(reqInfo).roomId;
	_handlerFactory->getRoomsManager().addUserToRoom(_user, id);
	return RequestResult(ResponsePacketSerializer::serializeOkResponse(),
		_handlerFactory->createRoomMemberRequestHandler(_user, id));
}
RequestResult MenuRequestHandler::getStatistics()
{
	StatisticsManager& statisticsManager = _handlerFactory->getStatisticsManager();
	return RequestResult(ResponsePacketSerializer::serializeResponse(
		GetStatisticsResponse(statisticsManager.getHighScore(), statisticsManager.getUserStatistics(_user.getUsername()))
	));
}
