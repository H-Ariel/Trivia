#include "SqliteDatabase.h"
#include "sqlite3.h"


SqliteDatabase::SqliteDatabase(const string& dbFilename)
	: _db(nullptr)
{
	struct stat s;
	bool fileExist = (stat(dbFilename.c_str(), &s) == 0);

	if (sqlite3_open(dbFilename.c_str(), &_db) != SQLITE_OK)
	{
		_db = nullptr;
		cout << "Failed to open DB" << endl;
		return;
	}

	// Initializes the data base.
	if (!fileExist)
	{
		// SQL statements to create the database
		const char* INIT_SQL_STATMENTS[] = {
			"CREATE TABLE USERS (username TEXT PRIMARY KEY NOT NULL, password TEXT NOT NULL, email TEXT NOT NULL);",
			"CREATE TABLE QUESTIONS(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, question TEXT NOT NULL, correct_ans TEXT NOT NULL, ans2 TEXT NOT NULL, ans3 TEXT NOT NULL, ans4 TEXT NOT NULL);"
			"CREATE TABLE STATISTICS(game_id INTEGER NOT NULL, username TEXT NOT NULL, score REAL NOT NULL);"
			"CREATE TABLE GAMES(id INTEGER NOT NULL, answer_timeout INTEGER NOT NULL, question_count INTEGER NOT NULL);"
		};
		
		const Question DEFAULT_QUESTIONS[] = {
			Question("0 + 1 = ", "1", { "a", "b", "c" }),
			Question("1 + 1 = ", "2", { "a", "b", "c" }),
			Question("2 + 1 = ", "3", { "a", "b", "c" }),
			Question("3 + 1 = ", "4", { "a", "b", "c" }),
			Question("4 + 1 = ", "5", { "a", "b", "c" }),
			Question("5 + 1 = ", "6", { "a", "b", "c" }),
			Question("6 + 1 = ", "7", { "a", "b", "c" }),
			Question("7 + 1 = ", "8", { "a", "b", "c" }),
			Question("8 + 1 = ", "9", { "a", "b", "c" }),
			Question("9 + 1 = ", "10", { "a", "b", "c" })
		};

		for (auto& i : INIT_SQL_STATMENTS)
		{
			try { runSqlStatements(i); }
			catch (...) {}
		}
		for (auto& q : DEFAULT_QUESTIONS)
		{
			try { addQuestion(q); }
			catch (...) {}
		}

#ifdef _DEBUG
		// add temporary users
		for (auto& i : {"a", "b", "c", "d", "e"})
		{
			addNewUser(i, "", "");
		}
#endif
	}
}
SqliteDatabase::~SqliteDatabase()
{
	sqlite3_close(_db);
}

bool SqliteDatabase::doesUserExist(const string& username)
{
	bool exist = false;
	runSqlStatements(format("SELECT * FROM USERS WHERE username=\"{}\";", username), SqliteDatabase::HasArgs, &exist);
	return exist;
}

bool SqliteDatabase::doesPasswordMatch(const string& username, const string& password)
{
	bool exist = false;
	runSqlStatements(format("SELECT * FROM USERS WHERE username=\"{}\" AND password=\"{}\";", username, password), SqliteDatabase::HasArgs, &exist);
	return exist;
}

void SqliteDatabase::addNewUser(const string& username, const string& password, const string& email)
{
	runSqlStatements(format("INSERT INTO USERS VALUES(\"{}\",\"{}\",\"{}\");", username, password, email));
}

int SqliteDatabase::getMaxQuestionsCount()
{
	int count = 0;
	runSqlStatements("SELECT COUNT(*) FROM QUESTIONS;", GetInt, &count);
	return count;
}

vector<Question> SqliteDatabase::getQuestions(int n)
{
	vector<Question> questions;
	runSqlStatements(format("SELECT * FROM QUESTIONS WHERE id <= {};", n), GetQuestions, &questions);
	return questions;
}

void SqliteDatabase::addQuestion(const Question& question)
{
	runSqlStatements(format("INSERT INTO QUESTIONS (question, correct_ans, ans2, ans3, ans4) \
		VALUES(\"{}\",\"{}\",\"{}\",\"{}\",\"{}\");",
		question.question, question.correctAnswer, question.wrongAnswers[0],
		question.wrongAnswers[1], question.wrongAnswers[2]));
}

void SqliteDatabase::addStatistics(const string& username, int gameId, float score)
{
	runSqlStatements(format("INSERT INTO STATISTICS VALUES({}, \"{}\", {} );", gameId, username, score));
}

UserStatistics SqliteDatabase::getUserStatistics(const string& username)
{
	UserStatistics stat;
	stat.username = username;
	runSqlStatements(format("SELECT * FROM STATISTICS WHERE username=\"{}\";", username), GetUserScore, &stat.score);
	return stat;
}

vector<UserStatistics> SqliteDatabase::getHighScore(int n)
{
	vector<UserStatistics> stats;
	runSqlStatements(format("SELECT * FROM STATISTICS ORDER BY score DESC LIMIT {};", n), GetHighScoreCallback, &stats);
	return stats;
}

int SqliteDatabase::getGameId()
{
	int id = 0;
	runSqlStatements("SELECT MAX(id) FROM GAMES;", GetInt, &id);
	return id + 1; // get next id
}

void SqliteDatabase::writeGameData(int id, int answerTimeout, int questionCount)
{
	char str[256] = {};
	sprintf(str, "INSERT INTO GAMES VALUES(%d, %d, %d);", id, answerTimeout, questionCount);
	runSqlStatements(str);
}


void SqliteDatabase::runSqlStatements(const string& sqlStatements, SQLite3Callback callback, void* dataToCallback)
{
	_dbMutex.lock();
	char* errMessage = nullptr;
	int err = sqlite3_exec(_db, sqlStatements.c_str(), callback, dataToCallback, &errMessage);
	_dbMutex.unlock();

	if (err != SQLITE_OK)
		throw Exception("Error in " __FUNCTION__ ":\nsqlStatements = " + sqlStatements + "\nerrMessage = " + (errMessage ? errMessage : ""));
}


/* Callback Functions: */

int SqliteDatabase::HasArgs(void* data, int argc, char** argv, char** azColName)
{
	*((bool*)data) = (argc != 0);
	return 0;
}
int SqliteDatabase::GetQuestions(void* data, int argc, char** argv, char** azColName)
{
	Question temp;
	string colName;

	for (int i = 0; i < argc; i++)
	{
		colName = azColName[i];
		if (colName == "question") temp.question = argv[i];
		else if (colName == "correct_ans") temp.correctAnswer = argv[i];
		else if (colName == "ans2") temp.wrongAnswers[0] = argv[i];
		else if (colName == "ans3") temp.wrongAnswers[1] = argv[i];
		else if (colName == "ans4") temp.wrongAnswers[2] = argv[i];
	}
	((vector<Question>*)data)->push_back(temp);
	return 0;
}
int SqliteDatabase::GetInt(void* data, int argc, char** argv, char** azColName)
{
	if (data && argc == 1 && argv[0])
		*((int*)data) = stoi(argv[0]);
	return 0;
}
int SqliteDatabase::GetUserScore(void* data, int argc, char** argv, char** azColName)
{
	string colName;

	for (int i = 0; i < argc; i++)
	{
		colName = azColName[i];
		if (colName == "score")
			*((float*)data) = stof(argv[i]);
	}

	return 0;
}
int SqliteDatabase::GetHighScoreCallback(void* data, int argc, char** argv, char** azColName)
{
	UserStatistics stat;
	string colName;

	for (int i = 0; i < argc; i++)
	{
		colName = azColName[i];
		if (colName == "score") stat.score = stof(argv[i]);
		else if (colName == "username") stat.username = argv[i];
	}

	((vector<UserStatistics>*)data)->push_back(stat);

	return 0;
}
