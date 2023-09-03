#include "GamesManager.h"


GamesManager::GamesManager(IDatabase* database)
	: database(database)
{
}

void GamesManager::createGame(unsigned int roomId, vector<LoggedUser> users, unsigned int questionCount)
{
	vector<Question> questions = database->getQuestions(questionCount);
	games[roomId] = allocNewSharedPtr<Game>(users, questions);
}

void GamesManager::deleteGame(unsigned int gameId)
{
	checkIfExists(gameId);
	games.erase(gameId);
}

void GamesManager::removePlayerFromGame(unsigned int gameId, const LoggedUser& user)
{
	checkIfExists(gameId);

	games[gameId]->removePlayer(user);
	if (games[gameId]->isEmpty())
		deleteGame(gameId);
}

Question GamesManager::getQuestionForUser(unsigned int gameId, const LoggedUser& user)
{
	checkIfExists(gameId);
	return games[gameId]->getQuestionForUser(user);
}

bool GamesManager::hasMoreQuestions(unsigned int gameId, const LoggedUser& user) const
{
	checkIfExists(gameId);
	return games.at(gameId)->hasMoreQuestions(user);
}

void GamesManager::submitAnswer(unsigned int gameId, const LoggedUser& user, unsigned int answerId)
{
	checkIfExists(gameId);
	games[gameId]->submitAnswer(user, answerId);
}

vector<PlayerResults> GamesManager::getGameResults(unsigned int gameId) const
{
	checkIfExists(gameId);
	return games.at(gameId)->getGameResults();
}


void GamesManager::checkIfExists(unsigned int gameId) const
{
	if (games.count(gameId) == 0)
		throw Exception("game does not exists. (id = " + to_string(gameId));
}
