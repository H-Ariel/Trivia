#include "LoginRequestHandler.h"
#include "PacketSerializer.h"


LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* handlerFactory, void* key)
	: _handlerFactory(handlerFactory), _key(key)
{
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	switch (reqInfo.msgCode)
	{
	case MessageCodes::Login: return login(reqInfo);
	case MessageCodes::Signup: return signup(reqInfo);
	}
	throw Exception("irrelevant request");
}

RequestResult LoginRequestHandler::login(const RequestInfo& reqInfo)
{
	LoginRequest req = RequestPacketDeserializer::deserializeLoginRequest(reqInfo);
	LoggedUser user = _handlerFactory->getLoginManager().login(_key, req.username, req.password);
	return RequestResult(ResponsePacketSerializer::serializeOkResponse(),
		_handlerFactory->createMenuRequestHandler(user));
}
RequestResult LoginRequestHandler::signup(const RequestInfo& reqInfo)
{
	SignupRequest req = RequestPacketDeserializer::deserializeSignupRequest(reqInfo);
	LoggedUser user = _handlerFactory->getLoginManager().signup(_key, req.username, req.password, req.email);
	return RequestResult(ResponsePacketSerializer::serializeOkResponse(),
		_handlerFactory->createMenuRequestHandler(user));
}
