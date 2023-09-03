#pragma once

#include "GeneralLibrary.h"


class IDatabase
{
public:
	virtual bool doesUserExist(string username) = 0;
	virtual bool doesPasswordMatch(string username, string password) = 0;
	virtual void addNewUser(string username, string password, string email) = 0;

	virtual unsigned int getMaxQuestionsCount() const = 0;
	virtual vector<Question> getQuestions(int n) = 0; // returns `n` questions
	virtual void addQuestion(Question question) = 0;

	virtual float getPlayerAverageAnswerTime(string username) = 0;
	virtual int getNumOfCorrectAnswers(string username) = 0;
	virtual int getNumOfTotalAnswers(string username) = 0;
	virtual int getNumOfPlayerGames(string username) = 0;
};
