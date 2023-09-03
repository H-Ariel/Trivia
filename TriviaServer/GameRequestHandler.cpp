#include "GameRequestHandler.h"
#include "PacketSerializer.h"
#include "MenuRequestHandler.h"


GameRequestHandler::GameRequestHandler(const LoggedUser& user, unsigned int gameId, RequestHandlerFactory& handlerFactory)
	: _user(user), _gameId(gameId), _handlerFactory(handlerFactory), _gamesManager(handlerFactory.getGamesManager())
{
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	switch (reqInfo.msgCode)
	{
	case MessageCodes::GetQuestion:
		result.response = ResponsePacketSerializer::serializeResponse(
			GetQuestionResponse(_gamesManager.getQuestionForUser(_gameId, _user)));
		result.changeHandler = false;
		break;

	case MessageCodes::SubmitAnswer:
		_gamesManager.submitAnswer(_gameId, _user,
			RequestPacketDeserializer::deserializeSubmitAnswerRequest(reqInfo).answerId);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.changeHandler = false;
		break;

	case MessageCodes::GetGameResults:
		result.response = ResponsePacketSerializer::serializeResponse(
			GetGameResultsResponse(_gamesManager.getGameResults(_gameId)));
		result.changeHandler = false;
		break;

	case MessageCodes::LeaveGame:
		_handlerFactory.getGamesManager().removePlayerFromGame(_gameId, _user);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = _handlerFactory.createMenuRequestHandler(_user);
		break;

	default:
		throw Exception("irrelevant request");
	}

	return result;
}
