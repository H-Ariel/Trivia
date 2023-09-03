#pragma once

#include "IDatabase.h"


struct sqlite3;


class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase(string dbFilename);
	~SqliteDatabase();

	bool doesUserExist(string username) override;
	bool doesPasswordMatch(string username, string password) override;
	void addNewUser(string username, string password, string email) override;

	unsigned int getMaxQuestionsCount() const override;
	vector<Question> getQuestions(int n) override;
	void addQuestion(Question question) override;

	float getPlayerAverageAnswerTime(string username) override;
	int getNumOfCorrectAnswers(string username) override;
	int getNumOfTotalAnswers(string username) override;
	int getNumOfPlayerGames(string username) override;


private:
	sqlite3* _db;

	typedef int (*SQLite3Callback)(void*, int, char**, char**);

	// Runs the inserted SQL statement.
	// @param callback: Indicates a function that will process the output.
	// @param dataToCallback: A parameter to be passed to the callback function.
	void runSqlStatements(string sqlStatements, SQLite3Callback callback = nullptr, void* dataToCallback = nullptr);

	/* Callback Functions: */

	static int HasArgs(void* data, int argc, char** argv, char** azColName);
	static int GetQuestions(void* data, int argc, char** argv, char** azColName);
};
