#include "structures.h"


RoomData::RoomData()
	: maxUsers(0), questionCount(0), answerTimeout(0), 
	id(-1),  isActive(false), hasGameStarted(false)
{
}
RoomData::RoomData(string name, unsigned int maxUsers, unsigned int questionCount, unsigned int answerTimeout)
	: name(name), maxUsers(maxUsers), questionCount(questionCount),
	answerTimeout(answerTimeout), id(-1), isActive(false), hasGameStarted(false)
{
}

PlayerGameData::PlayerGameData()
	: currentQuestionId(0), correctAnswerCount(0), wrongAnswerCount(0), 
	averangeAnswerTime(0), isActive(true)
{
}

Question::Question(string question, string correctAnswer, vector<string> wrongAnswers)
	: question(question), correctAnswer(correctAnswer), wrongAnswers(wrongAnswers)
{
}


GetRoomsResponse::GetRoomsResponse(vector<tuple<unsigned int, string>> rooms)
	: rooms(rooms)
{
}

GetQuestionResponse::GetQuestionResponse(Question q)
	: question(q.question)
{
	answers.push_back(q.correctAnswer);
	for (const string& a : q.wrongAnswers)
		answers.push_back(a);
}

GetGameResultsResponse::GetGameResultsResponse(vector<PlayerResults> results)
	: results(results)
{
}

GetRoomStateResponse::GetRoomStateResponse(RoomData roomData)
	: name(roomData.name), answerTimeout(roomData.answerTimeout),
	hasGameStarted(roomData.hasGameStarted), isActive(roomData.isActive),
	questionCount(roomData.questionCount)
{
}
