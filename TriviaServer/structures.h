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
	RoomData() = default;
	RoomData(
		const string& name,
		unsigned int maxUsers,
		unsigned int questionCount,
		unsigned int answerTimeout
	);
	RoomData(const CreateRoomRequest&);

	string name;
	unsigned int id = -1;
	unsigned int maxUsers = 0;
	unsigned int questionCount = 0;
	unsigned int answerTimeout = 0; // in seconds
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
	string username;
	float score = 0;
};

struct PlayerGameData
{
	unsigned int currentQuestionId = 0;
	unsigned int correctAnswerCount = 0;
	unsigned int wrongAnswerCount = 0;
	float averangeAnswerTime = 0;
	bool isActive = true; // inactive player is a player who leaves the game in the middle
};

struct PlayerResults
{
	string username;
	unsigned int correctAnswerCount = 0;
	unsigned int wrongAnswerCount = 0; // TODO: maybe not needed because it's the same as "totalAnswersCount-correctAnswerCount"
	float averangeAnswerTime = 0;
};



/* Response Struct-s */

struct ErrorResponse
{
	ErrorResponse(const string& message) 
		: message(message) {}
	const string message;
};

struct GetRoomsResponse
{
	GetRoomsResponse(const vector<tuple<unsigned int, string>>& rooms)
		: rooms(rooms) {}
	vector<tuple<unsigned int, string>> rooms;
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
	unsigned int questionCount;
	unsigned int answerTimeout;
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
	GetGameResultsResponse(const vector<PlayerResults>& results)
		: results(results) {}
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
