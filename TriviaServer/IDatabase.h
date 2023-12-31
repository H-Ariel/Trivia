#pragma once

#include "GeneralLibrary.h"


class IDatabase
{
public:
	virtual bool doesUserExist(const string& username) = 0;
	virtual bool doesPasswordMatch(const string& username, const string& password) = 0;
	virtual void addNewUser(const string& username, const string& password, const string& email) = 0;

	virtual int getMaxQuestionsCount() = 0;
	virtual vector<Question> getQuestions(int n) = 0; // returns `n` questions
	virtual void addQuestion(const Question& question) = 0;

	virtual void addStatistics(const string& username, int gameId, float score) = 0;
	virtual UserStatistics getUserStatistics(const string& username) = 0;
	virtual vector<UserStatistics> getHighScore(int n) = 0; // returns n users with the highest score.

	virtual int getGameId() = 0;
	virtual void writeGameData(int id, int answerTimeout, int questionCount) = 0;

	
protected:
	mutex _dbMutex;
};
