#include "GameRequestHandler.h"
#include "PacketSerializer.h"
#include "MenuRequestHandler.h"


GameRequestHandler::GameRequestHandler(const LoggedUser& user, unsigned int gameId, RequestHandlerFactory& handlerFactory)
	: user(user), gameId(gameId), handlerFactory(handlerFactory), gamesManager(handlerFactory.getGamesManager())
{
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	switch (reqInfo.msgCode)
	{
	case MessageCodes::GetQuestion: {
		Question q = gamesManager.getQuestionForUser(gameId, user);
		result.response = ResponsePacketSerializer::serializeResponse(GetQuestionResponse(q));
		result.changeHandler = false;
	}	break;

	case MessageCodes::SubmitAnswer: {
		SubmitAnswerRequest req = RequestPacketDeserializer::deserializeSubmitAnswerRequest(reqInfo);
		gamesManager.submitAnswer(gameId, user, req.answerId);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.changeHandler = false;
	}	break;

	case MessageCodes::GetGameResults: {
		GetGameResultsResponse resp(gamesManager.getGameResults(gameId));
		result.response = ResponsePacketSerializer::serializeResponse(resp);
		result.changeHandler = false;
	}	break;

	case MessageCodes::LeaveGame: {
		handlerFactory.getGamesManager().removePlayerFromGame(gameId, user);
		result.response = ResponsePacketSerializer::serializeOkResponse();
		result.newHandler = handlerFactory.createMenuRequestHandler(user);
	}	break;

	default:
		throw Exception("irrelevant request");
	}

	return result;
}
