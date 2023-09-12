#pragma once

#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <cstdint>

using namespace std;


using Buffer = vector<uint8_t>;

struct RequestInfo
{
	uint8_t msgCode;
	Buffer msgData;
};


/* Data Struct-s */

struct CreateRoomRequest;

struct RoomData
{
	RoomData();
	RoomData(
		string name,
		unsigned int maxUsers,
		unsigned int questionCount,
		unsigned int answerTimeout
	);
	RoomData(const CreateRoomRequest&);

	string name;
	unsigned int id;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout; // in seconds
	bool isActive;
	bool hasGameStarted;
};


struct Question
{
	Question() = default; // TODO
	Question(string question, string correctAnswer, vector<string> wrongAnswers);

	string question;
	string correctAnswer;
	string wrongAnswers[3]; // totaly 4 answers
};

struct UserStatistics
{
	UserStatistics() : score(0) {}

	string username;
	float score;
};

struct PlayerGameData
{
	PlayerGameData();

	unsigned int currentQuestionId;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	float averangeAnswerTime;
	bool isActive; // inactive player is a player who leaves the game in the middle
};

struct PlayerResults
{
	PlayerResults() : correctAnswerCount(0), wrongAnswerCount(0), averangeAnswerTime(0) {}

	string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount; // TODO: maybe not needed because it's the same as "totalAnswersCount-correctAnswerCount"
	float averangeAnswerTime;
};



/* Response Struct-s */

struct ErrorResponse
{
	ErrorResponse(string message) : message(message) {}
	const string message;
};

struct GetRoomsResponse
{
	GetRoomsResponse(vector<tuple<unsigned int, string>> rooms);
	vector<tuple<unsigned int, string>> rooms;
};

struct GetStatisticsResponse
{
	GetStatisticsResponse(vector<UserStatistics> highScore, UserStatistics userStat);
	
	vector<UserStatistics> highScore;
	UserStatistics userStat;
};

struct GetRoomStateResponse
{
	GetRoomStateResponse(const RoomData& roomData);

	string name;
	vector<string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
	bool isActive;
	bool hasGameStarted;
};

struct GetQuestionResponse
{
	GetQuestionResponse(Question);

	string question;
	vector<string> answers; // the first answer is the correct, the other are wrong
};

struct GetGameResultsResponse
{
	GetGameResultsResponse(vector<PlayerResults> results);
	vector<PlayerResults> results;
};



/* Request Struct-s */

struct LoginRequest
{
	string username;
	string password;
};
struct SignupRequest
{
	string username;
	string password;
	string email;
};

struct CreateRoomRequest
{
	string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};
struct JoinRoomRequest
{
	unsigned int roomId;
};

struct SubmitAnswerRequest
{
	unsigned int answerId;
};
