#pragma once

#include "IDatabase.h"
#include "Game.h"
#include "Room.h"


class GamesManager
{
public:
	GamesManager(IDatabase* database);

	void createGame(const RoomData& roomData, const vector<LoggedUser>& users);
	void removePlayerFromGame(int gameId, const LoggedUser& user);
	Question getQuestionForUser(int gameId, const LoggedUser& user);
	bool hasMoreQuestions(int gameId, const LoggedUser& user) const;
	void submitAnswer(int gameId, const LoggedUser& user, int answerId);
	vector<UserStatistics> getGameResults(int gameId) const;


private:
	void deleteGame(int gameId);
	void checkIfExists(int gameId) const;

	map<int, shared_ptr<Game>> _games; // [id]=game (the game ID is same to the room ID)
	IDatabase* const _database;
};
