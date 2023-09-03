#include "LoginRequestHandler.h"
#include "PacketSerializer.h"
#include "MenuRequestHandler.h"


LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory, SOCKET sock)
	: _handlerFactory(handlerFactory), _loginManager(handlerFactory.getLoginManager()), _sock(sock)
{
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	switch (reqInfo.msgCode) // msg code
	{
	case MessageCodes::Login: {
		LoginRequest req = RequestPacketDeserializer::deserializeLoginRequest(reqInfo);
		const LoggedUser& user = _loginManager.login(_sock, req.username, req.password);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = _handlerFactory.createMenuRequestHandler(user);
	}	break;

	case MessageCodes::Signup: {
		SignupRequest req = RequestPacketDeserializer::deserializeSignupRequest(reqInfo);
		const LoggedUser& user = _loginManager.signup(_sock, req.username, req.password, req.email);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = _handlerFactory.createMenuRequestHandler(user);
	}	break;

	default:
		throw Exception("irrelevant request");
	}

	return result;
}
