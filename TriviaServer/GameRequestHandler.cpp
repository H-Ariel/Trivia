#include "GameRequestHandler.h"
#include "PacketSerializer.h"


GameRequestHandler::GameRequestHandler(RequestHandlerFactory* handlerFactory, const LoggedUser& user, int gameId)
	: _user(user), _gameId(gameId), _handlerFactory(handlerFactory)
{
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	switch (reqInfo.msgCode)
	{
	case MessageCodes::GetQuestion: return getQuestion();
	case MessageCodes::SubmitAnswer: return submitAnswer(reqInfo);
	case MessageCodes::GetGameResults: return getGameResults();
	case MessageCodes::LeaveGame: return leaveGame();
	}
	throw Exception("irrelevant request");
}

RequestResult GameRequestHandler::getQuestion()
{
	return RequestResult(ResponsePacketSerializer::serializeResponse(
		GetQuestionResponse(_handlerFactory->getGamesManager().getQuestionForUser(_gameId, _user))));
}
RequestResult GameRequestHandler::submitAnswer(const RequestInfo& reqInfo)
{
	_handlerFactory->getGamesManager().submitAnswer(_gameId, _user,
		RequestPacketDeserializer::deserializeSubmitAnswerRequest(reqInfo).answerId);
	return RequestResult(ResponsePacketSerializer::serializeOkResponse());
}
RequestResult GameRequestHandler::getGameResults()
{
	return RequestResult(ResponsePacketSerializer::serializeResponse(
		GetGameResultsResponse(_handlerFactory->getGamesManager().getGameResults(_gameId))));
}
RequestResult GameRequestHandler::leaveGame()
{
	_handlerFactory->getGamesManager().removePlayerFromGame(_gameId, _user);
	return RequestResult(ResponsePacketSerializer::serializeOkResponse(),
		_handlerFactory->createMenuRequestHandler(_user));
}
