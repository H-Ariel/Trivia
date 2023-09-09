#pragma once

#include "GeneralLibrary.h"


class IDatabase
{
public:
	virtual bool doesUserExist(const string& username) = 0;
	virtual bool doesPasswordMatch(const string& username, const string& password) = 0;
	virtual void addNewUser(const string& username, const string& password, const string& email) = 0;

	virtual unsigned int getMaxQuestionsCount() = 0;
	virtual vector<Question> getQuestions(unsigned int n) = 0; // returns `n` questions
	virtual void addQuestion(const Question& question) = 0;

	virtual float getPlayerAverageAnswerTime(const string& username) = 0;
	virtual int getNumOfCorrectAnswers(const string& username) = 0;
	virtual int getNumOfTotalAnswers(const string& username) = 0;
	virtual int getNumOfPlayerGames(const string& username) = 0;
};
