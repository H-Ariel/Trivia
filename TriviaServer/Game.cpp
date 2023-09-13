#include "Game.h"


Game::Game(const vector<LoggedUser>& users, const vector<Question>& questions)
	: questions(questions)
{
	for (const LoggedUser& user : users)
		players[user] = PlayerGameData();
}

Question Game::getQuestionForUser(const LoggedUser& user)
{
	if (!hasMoreQuestions(user))
		throw Exception("no more question");
	return questions[players[user].currentQuestionId];
}

void Game::submitAnswer(const LoggedUser& user, unsigned int answerId)
{
	if (answerId == 0)
	{
		// we send the answers when the first answer is the correct -so 0 is the correct answer id
		players[user].correctAnswerCount += 1;
	}
	else
	{
		players[user].wrongAnswerCount += 1;
	}

	// TODO: players[user].averangeAnswerTime

	players[user].currentQuestionId += 1; // get next question
}

void Game::removePlayer(const LoggedUser& user)
{
	players[user].isActive = false;
}

bool Game::hasMoreQuestions(const LoggedUser& user) const
{
	return players.at(user).currentQuestionId < questions.size();
}

vector<PlayerResults> Game::getGameResults() const
{
	for (const auto& i : players)
	{
		if (i.second.isActive) // if the player inactive we ignore him
			if (i.second.currentQuestionId < (unsigned int)questions.size()) // player not answer all questions
				throw Exception("Not all players finished the game");
	}

	vector<PlayerResults> res;

	for (auto& i : players)
	{
		PlayerResults pr;
		pr.username = i.first.getUsername();
		pr.averangeAnswerTime = i.second.averangeAnswerTime;
		pr.correctAnswerCount = i.second.correctAnswerCount;
		pr.wrongAnswerCount = i.second.wrongAnswerCount;

		res.push_back(pr);
	}

	return res;
}

bool Game::isEmpty() const
{
	for (auto& i : players)
	{
		if (i.second.isActive)
		{
			return false;
		}
	}

	return true;
}
