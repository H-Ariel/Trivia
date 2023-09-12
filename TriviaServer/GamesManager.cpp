#include "GamesManager.h"


GamesManager::GamesManager(IDatabase* database)
	: _database(database)
{
}

void GamesManager::createGame(const RoomData& roomData, const vector<LoggedUser>& users)
{
	_database->writeGameData(roomData.id, roomData.answerTimeout, roomData.questionCount);
	_games[roomData.id] = allocNewSharedPtr<Game>(users, _database->getQuestions(roomData.questionCount));
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
	vector<PlayerResults> results = _games.at(gameId)->getGameResults();

	/*
	// TODO: calc total score

	void calcScore(unsigned int gameCount, float averangeAnswerTime, unsigned int correctAnswersCount, unsigned int totalAnswersCount)
	{
		return (float)correctAnswersCount / totalAnswersCount * 100 * gameCount / averangeAnswerTime;
	}
	*/

	for (auto& r : results)
	{
		_database->addStatistics(r.username, gameId, (float)r.correctAnswerCount / (r.correctAnswerCount + r.wrongAnswerCount));
	}

	return results;
}

void GamesManager::checkIfExists(unsigned int gameId) const
{
	if (_games.count(gameId) == 0)
		throw Exception("game does not exists. (id = " + to_string(gameId));
}
