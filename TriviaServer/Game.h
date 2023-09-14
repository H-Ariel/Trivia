#pragma once

#include "LoggedUser.h"


class Game
{
public:
	Game() = default;
	Game(const vector<LoggedUser>& users, const vector<Question>& questions);

	Question getQuestionForUser(const LoggedUser& user);
	void submitAnswer(const LoggedUser& user, int answerId);
	void removePlayer(const LoggedUser& user);

	bool hasMoreQuestions(const LoggedUser& user) const;
	vector<UserStatistics> getGameResults() const;
	bool isEmpty() const;


private:
	const vector<Question> questions;
	map<LoggedUser, PlayerGameData> players;
};
