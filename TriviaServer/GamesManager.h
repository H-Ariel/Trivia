#pragma once

#include "IDatabase.h"
#include "Game.h"
#include "Room.h"


class GamesManager
{
public:
	GamesManager(IDatabase* database);

	void createGame(unsigned int roomId, vector<LoggedUser> users, unsigned int questionCount);
	
	void removePlayerFromGame(unsigned int gameId, const LoggedUser& user);
	Question getQuestionForUser(unsigned int gameId, const LoggedUser& user);
	bool hasMoreQuestions(unsigned int gameId, const LoggedUser& user) const;
	void submitAnswer(unsigned int gameId, const LoggedUser& user, unsigned int answerId);
	vector<PlayerResults> getGameResults(unsigned int gameId) const;


private:
	void deleteGame(unsigned int gameId);
	void checkIfExists(unsigned int gameId) const;

	IDatabase* const database;
	map<unsigned int, shared_ptr<Game>> games; // [id]=game  (the game ID is same to the room ID)
};
