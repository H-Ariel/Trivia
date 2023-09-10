#include "LoginRequestHandler.h"
#include "PacketSerializer.h"
#include "MenuRequestHandler.h"


LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory, int id)
	: _handlerFactory(handlerFactory), _loginManager(handlerFactory.getLoginManager()), _id(id)
{
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	switch (reqInfo.msgCode)
	{
	case MessageCodes::Login: {
		LoginRequest req = RequestPacketDeserializer::deserializeLoginRequest(reqInfo);
		result.newHandler = _handlerFactory.createMenuRequestHandler(_loginManager.login(_id, req.username, req.password));
		result.response = ResponsePacketSerializer::serializeOkResponse();
	}	break;

	case MessageCodes::Signup: {
		SignupRequest req = RequestPacketDeserializer::deserializeSignupRequest(reqInfo);
		result.newHandler = _handlerFactory.createMenuRequestHandler(_loginManager.signup(_id, req.username, req.password, req.email));
		result.response = ResponsePacketSerializer::serializeOkResponse();
	}	break;

	default:
		throw Exception("irrelevant request");
	}

	return result;
}
