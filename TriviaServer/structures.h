#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <chrono>

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
	RoomData(
		const string& name = "",
		int maxUsers = 0,
		int questionCount = 0,
		int answerTimeout = 0
	);
	RoomData(const CreateRoomRequest&);

	string name;
	int id = -1;
	int maxUsers = 0;
	int questionCount = 0;
	int answerTimeout = 0; // in seconds
	bool isActive = false;
	bool hasGameStarted = false;
};


struct Question
{
	Question() = default;
	Question(const string& question, const string& correctAnswer, const vector<string>& wrongAnswers);

	string question;
	string correctAnswer;
	string wrongAnswers[3]; // totaly 4 answers
};

struct UserStatistics
{
	UserStatistics(const string& username = "", float score = 0)
		: username(username), score(score) {}
	string username;
	float score = 0;
};

struct PlayerGameData
{
	chrono::system_clock::time_point timeGetQuestion; // save current time when get question for calculate the averange answer time
	int currentQuestionId = 0;
	int correctAnswerCount = 0;
	float averangeAnswerTime = 0;
	bool isActive = true; // inactive player is a player who leaves the game in the middle
};


/* Response Struct-s */

struct ErrorResponse
{
	ErrorResponse(const string& message) 
		: message(message) {}
	string message;
};

struct GetRoomsResponse
{
	GetRoomsResponse(const map<int, string>& rooms)
		: rooms(rooms) {}
	map<int, string> rooms;
};

struct GetStatisticsResponse
{
	GetStatisticsResponse(const vector<UserStatistics>& highScore, const UserStatistics& userStat)
		: highScore(highScore), userStat(userStat) {}

	vector<UserStatistics> highScore;
	UserStatistics userStat;
};

struct GetRoomStateResponse
{
	GetRoomStateResponse(const RoomData& roomData);

	string name;
	vector<string> players;
	int questionCount;
	int answerTimeout;
	bool isActive;
	bool hasGameStarted;
};

struct GetQuestionResponse
{
	GetQuestionResponse(const Question&);

	string question;
	vector<string> answers; // the first answer is the correct, the other are wrong
};

struct GetGameResultsResponse
{
	GetGameResultsResponse(const vector<UserStatistics>& results)
		: results(results) {}
	vector<UserStatistics> results;
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
	int maxUsers;
	int questionCount;
	int answerTimeout;
};
struct JoinRoomRequest
{
	int roomId;
};

struct SubmitAnswerRequest
{
	int answerId;
};
