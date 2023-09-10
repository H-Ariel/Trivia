#include "SqliteDatabase.h"
#include "sqlite3.h"


#define DEFAULT_QUESTIONS_COUNT 10


SqliteDatabase::SqliteDatabase(const string& dbFilename)
	: _db(nullptr)
{
	bool fileExist = doesFileExist(dbFilename);

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
			"CREATE TABLE QUESTIONS(question_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, question TEXT NOT NULL, correct_ans TEXT NOT NULL, ans2 TEXT NOT NULL, ans3 TEXT NOT NULL, ans4 TEXT NOT NULL);"
			"CREATE TABLE STATISTICS(game_id INTEGER NOT NULL, username TEXT NOT NULL, averange_answer_time REAL NOT NULL, correct_answers_count INTEGER NOT NULL, total_answers_count INTEGER NOT NULL);"
		};
		
		const Question DEFAULT_QUESTIONS[DEFAULT_QUESTIONS_COUNT] = {
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
	runSqlStatements("SELECT * FROM USERS WHERE username=\"" + username + "\";", SqliteDatabase::HasArgs, &exist);
	return exist;
}

bool SqliteDatabase::doesPasswordMatch(const string& username, const string& password)
{
	bool exist = false;
	runSqlStatements("SELECT * FROM USERS WHERE username=\"" + username + "\" AND password=\"" + password + "\";", SqliteDatabase::HasArgs, &exist);
	return exist;
}

void SqliteDatabase::addNewUser(const string& username, const string& password, const string& email)
{
	runSqlStatements("INSERT INTO USERS VALUES(\"" + username + "\",\"" + password + "\",\"" + email + "\");");
}

unsigned int SqliteDatabase::getMaxQuestionsCount()
{
	unsigned int count = 0;
	runSqlStatements("SELECT COUNT(*) FROM QUESTIONS;", SqliteDatabase::GetInt, &count);
	return count;
}

vector<Question> SqliteDatabase::getQuestions(unsigned int n)
{
	vector<Question> questions;
	runSqlStatements("SELECT * FROM QUESTIONS WHERE question_id <= " + to_string(n) + ";", SqliteDatabase::GetQuestions, &questions);
	return questions;
}

void SqliteDatabase::addQuestion(const Question& question)
{
	string possibleAnswers;

	possibleAnswers += '"' + question.correctAnswer + '"';
	for (const string& ans : question.wrongAnswers)
		possibleAnswers += ",\"" + ans + '"';

	runSqlStatements("INSERT INTO QUESTIONS (question, correct_ans, ans2, ans3, ans4) VALUES (\"" + question.question + "\"," + possibleAnswers + ");");
}

void SqliteDatabase::addStatistics(const string& username, unsigned int gameId, float averangeAnswerTime, unsigned int correctAnswersCount, unsigned int totalAnswersCount)
{
	char str[256] = {};
	sprintf(str, "INSERT INTO STATISTICS VALUES(%d, \"%s\", %f, %d, %d);", gameId,
		username.c_str(), averangeAnswerTime, correctAnswersCount, totalAnswersCount);
	runSqlStatements(str);
}

UserStatistics SqliteDatabase::getUserStatistics(const string& username)
{
	throw Exception(__FUNCTION__ " DOES NOT IMPLETED");
}

vector<UserStatistics> SqliteDatabase::getHighScore(int n)
{
	throw Exception(__FUNCTION__ " DOES NOT IMPLETED");
}


void SqliteDatabase::runSqlStatements(const string& sqlStatements, SQLite3Callback callback, void* dataToCallback)
{
	char* errMessage = nullptr;
	if (sqlite3_exec(_db, sqlStatements.c_str(), callback, dataToCallback, &errMessage) != SQLITE_OK)
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
		else if (colName == "ans2" || colName == "ans3" || colName == "ans4") temp.wrongAnswers.push_back(argv[i]);
		else if (colName == "correct_ans") temp.correctAnswer = argv[i];
	}
	((vector<Question>*)data)->push_back(temp);
	return 0;
}
int SqliteDatabase::GetInt(void* data, int argc, char** argv, char** azColName)
{
	if (argc == 1)
		*((unsigned int*)data) = stoi(argv[0]);
	return 0;
}
