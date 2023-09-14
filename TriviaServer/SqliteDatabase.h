#pragma once

#include "IDatabase.h"


struct sqlite3;


class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase(const string& dbFilename);
	~SqliteDatabase();

	bool doesUserExist(const string& username) override;
	bool doesPasswordMatch(const string& username, const string& password) override;
	void addNewUser(const string& username, const string& password, const string& email) override;

	int getMaxQuestionsCount() override;
	vector<Question> getQuestions(int n) override;
	void addQuestion(const Question& question) override;

	void addStatistics(const string& username, int gameId, float score) override;
	UserStatistics getUserStatistics(const string& username) override;
	vector<UserStatistics> getHighScore(int n) override;

	int getGameId() override;
	void writeGameData(int id, int answerTimeout, int questionCount) override;

private:
	sqlite3* _db;

	typedef int (*SQLite3Callback)(void*, int, char**, char**);

	// Runs the inserted SQL statement.
	// @param callback: Indicates a function that will process the output.
	// @param dataToCallback: A parameter to be passed to the callback function.
	void runSqlStatements(const string& sqlStatements, SQLite3Callback callback = nullptr, void* dataToCallback = nullptr);

	/* Callback Functions: */

	static int HasArgs(void* data, int argc, char** argv, char** azColName);
	static int GetQuestions(void* data, int argc, char** argv, char** azColName);
	static int GetInt(void* data, int argc, char** argv, char** azColName);
	static int GetUserScore(void* data, int argc, char** argv, char** azColName);
	static int GetHighScoreCallback(void* data, int argc, char** argv, char** azColName);
};
