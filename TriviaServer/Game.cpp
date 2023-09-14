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

	players[user].timeGetQuestion = chrono::system_clock::now();
	return questions[players[user].currentQuestionId];
}

void Game::submitAnswer(const LoggedUser& user, int answerId)
{
	float duration = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - players[user].timeGetQuestion).count() / 1000.f;

	if (answerId == 0)
	{
		// we send the answers when the first answer is the correct - so 0 is the correct-answer's id
		players[user].correctAnswerCount += 1;
	}

	players[user].currentQuestionId += 1; // get next question
	players[user].averangeAnswerTime = (players[user].averangeAnswerTime * (players[user].currentQuestionId - 1) + duration) / players[user].currentQuestionId;
}

void Game::removePlayer(const LoggedUser& user)
{
	players[user].isActive = false;
}

bool Game::hasMoreQuestions(const LoggedUser& user) const
{
	return players.at(user).currentQuestionId < questions.size();
}

vector<UserStatistics> Game::getGameResults() const
{
	for (const auto& i : players)
	{
		if (i.second.isActive) // if the player inactive we ignore him
			if (i.second.currentQuestionId < (int)questions.size()) // player not answer all questions
				throw Exception("Not all players finished the game");
	}

	vector<UserStatistics> res;

	for (auto& i : players)
	{
		res.push_back(UserStatistics(i.first.getUsername(),
			i.second.correctAnswerCount * i.second.averangeAnswerTime * 1000));
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
