#pragma once

#include "LoggedUser.h"


class Game
{
public:
	Game() = default;
	Game(vector<LoggedUser> users, vector<Question> questions);

	Question getQuestionForUser(const LoggedUser& user);
	void submitAnswer(const LoggedUser& user, unsigned int answerId);
	void removePlayer(const LoggedUser& user);

	bool hasMoreQuestions(const LoggedUser& user) const;
	vector<PlayerResults> getGameResults() const;
	bool isEmpty() const;


private:
	const vector<Question> questions;
	map<LoggedUser, PlayerGameData> players;
};
