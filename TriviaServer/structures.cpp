#include "structures.h"


RoomData::RoomData(const string& name, int maxUsers, int questionCount, int answerTimeout)
	: name(name), maxUsers(maxUsers), questionCount(questionCount), answerTimeout(answerTimeout)
{
}
RoomData::RoomData(const CreateRoomRequest& req)
	: RoomData(req.roomName, req.maxUsers, req.questionCount, req.answerTimeout)
{
}

Question::Question(const string& question, const string& correctAnswer, const vector<string>& wrongAnswers)
	: question(question), correctAnswer(correctAnswer)
{
	for (int i = 0; i < 3; i++)
		this->wrongAnswers[i] = wrongAnswers[i];
}

GetRoomStateResponse::GetRoomStateResponse(const RoomData& roomData)
	: name(roomData.name), answerTimeout(roomData.answerTimeout),
	hasGameStarted(roomData.hasGameStarted), isActive(roomData.isActive),
	questionCount(roomData.questionCount)
{
}

GetQuestionResponse::GetQuestionResponse(const Question& q)
	: question(q.question)
{
	answers.push_back(q.correctAnswer);
	for (const string& a : q.wrongAnswers)
		answers.push_back(a);
}
