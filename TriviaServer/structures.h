#pragma once

#include <string>
#include <vector>
#include <list>
#include <tuple>

using namespace std;



/* Data Struct-s */

struct RoomData
{
	RoomData();
	RoomData(
		string name,
		unsigned int maxUsers,
		unsigned int questionCount,
		unsigned int answerTimeout
	);

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
	Question() = default;
	Question(string question, string correctAnswer, vector<string> wrongAnswers);

	string question;
	string correctAnswer;
	vector<string> wrongAnswers;
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
	string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
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

struct GetHighScoreResponse
{
	vector<string> statistics;
};
struct GetStatisticsResponse
{
	vector<string> statistics;
};

struct GetRoomStateResponse
{
	GetRoomStateResponse(RoomData roomData);
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
