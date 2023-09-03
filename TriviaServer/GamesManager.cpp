#include "GamesManager.h"


GamesManager::GamesManager(IDatabase* database)
	: _database(database)
{
}

void GamesManager::createGame(unsigned int roomId, vector<LoggedUser> users, unsigned int questionCount)
{
	_games[roomId] = allocNewSharedPtr<Game>(users, _database->getQuestions(questionCount));
}

void GamesManager::deleteGame(unsigned int gameId)
{
	checkIfExists(gameId);
	_games.erase(gameId);
}

void GamesManager::removePlayerFromGame(unsigned int gameId, const LoggedUser& user)
{
	checkIfExists(gameId);

	_games[gameId]->removePlayer(user);
	if (_games[gameId]->isEmpty())
		deleteGame(gameId);
}

Question GamesManager::getQuestionForUser(unsigned int gameId, const LoggedUser& user)
{
	checkIfExists(gameId);
	return _games[gameId]->getQuestionForUser(user);
}

bool GamesManager::hasMoreQuestions(unsigned int gameId, const LoggedUser& user) const
{
	checkIfExists(gameId);
	return _games.at(gameId)->hasMoreQuestions(user);
}

void GamesManager::submitAnswer(unsigned int gameId, const LoggedUser& user, unsigned int answerId)
{
	checkIfExists(gameId);
	_games[gameId]->submitAnswer(user, answerId);
}

vector<PlayerResults> GamesManager::getGameResults(unsigned int gameId) const
{
	checkIfExists(gameId);
	return _games.at(gameId)->getGameResults();
}

void GamesManager::checkIfExists(unsigned int gameId) const
{
	if (_games.count(gameId) == 0)
		throw Exception("game does not exists. (id = " + to_string(gameId));
}
