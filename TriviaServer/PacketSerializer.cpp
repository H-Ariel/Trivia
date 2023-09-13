#include "PacketSerializer.h"
#include "json.hpp"

using nlohmann::json;


inline json JsonFromBuffer(const RequestInfo& reqInfo)
{
	return json::parse(string(reqInfo.msgData.begin(), reqInfo.msgData.end()));
}

RequestInfo BufferFromJson(const json& j, MessageCodes code = MessageCodes::OK)
{
	const string data(j.dump());

	RequestInfo reqInfo;
	
	reqInfo.msgCode = code;
	reqInfo.msgData = Buffer(begin(data), end(data));

	return reqInfo;
}


RequestInfo ResponsePacketSerializer::serializeOkResponse()
{
	return BufferFromJson({});
}

RequestInfo ResponsePacketSerializer::serializeErrorResponse(const string& message)
{
	return BufferFromJson({ { "message", message } }, MessageCodes::Error);
}


RequestInfo ResponsePacketSerializer::serializeResponse(const GetRoomsResponse& resp)
{
	vector<json> v;

	for (const tuple<unsigned int, string>& i : resp.rooms)
	{
		v.push_back({
			{ "id", get<0>(i) },
			{ "name", get<1>(i) },
		});
	}

	return BufferFromJson({ { "rooms", v } });
}

RequestInfo ResponsePacketSerializer::serializeResponse(const GetStatisticsResponse& resp)
{
	vector<json> highScore;
	for (auto& i : resp.highScore)
	{
		json tmp;
		tmp["username"] = i.username;
		tmp["score"] = i.score;
		highScore.push_back(tmp);
	}

	return BufferFromJson({
		{
			"userStat", {
				{ "username", resp.userStat.username },
				{ "score", resp.userStat.score}
			} 
		},
		{ "highScore", highScore }
	});
}

RequestInfo ResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& resp)
{
	return BufferFromJson({
		{ "answerTimeout", resp.answerTimeout },
		{ "hasGameStarted", resp.hasGameStarted },
		{ "isActive", resp.isActive },
		{ "name", resp.name },
		{ "players", resp.players },
		{ "questionCount", resp.questionCount },
	});
}

RequestInfo ResponsePacketSerializer::serializeResponse(const GetQuestionResponse& resp)
{
	return BufferFromJson({
		{ "answers", resp.answers },
		{ "question", resp.question }
	});
}

RequestInfo ResponsePacketSerializer::serializeResponse(const GetGameResultsResponse& resp)
{
	vector<json> v;

	for (const PlayerResults& i : resp.results)
	{
		v.push_back({
			{ "averangeAnswerTime", i.averangeAnswerTime },
			{ "correctAnswerCount", i.correctAnswerCount },
			{ "username", i.username },
			{ "wrongAnswerCount", i.wrongAnswerCount }
		});
	}

	return BufferFromJson({ { "results", v } });
}


#define CheckIfJsonContains(k) if (!j.contains(k)) throw Exception("json does not contains key: " k);


LoginRequest RequestPacketDeserializer::deserializeLoginRequest(const RequestInfo& reqInfo)
{
	const json j = JsonFromBuffer(reqInfo);
	CheckIfJsonContains("username");
	CheckIfJsonContains("password");
	LoginRequest r;
	r.username = j["username"];
	r.password = j["password"];
	return r;
}

SignupRequest RequestPacketDeserializer::deserializeSignupRequest(const RequestInfo& reqInfo)
{
	const json j = JsonFromBuffer(reqInfo);
	CheckIfJsonContains("username");
	CheckIfJsonContains("password");
	CheckIfJsonContains("email");
	SignupRequest r;
	r.username = j["username"];
	r.password = j["password"];
	r.email = j["email"];
	return r;
}

CreateRoomRequest RequestPacketDeserializer::deserializeCreateRoomRequest(const RequestInfo& reqInfo)
{
	const json j = JsonFromBuffer(reqInfo);
	CheckIfJsonContains("answerTimeout");
	CheckIfJsonContains("maxUsers");
	CheckIfJsonContains("questionCount");
	CheckIfJsonContains("roomName");
	CreateRoomRequest r;
	r.answerTimeout = j["answerTimeout"];
	r.maxUsers = j["maxUsers"];
	r.questionCount = j["questionCount"];
	r.roomName = j["roomName"];
	return r;
}

JoinRoomRequest RequestPacketDeserializer::deserializeJoinRoomRequest(const RequestInfo& reqInfo)
{
	const json j = JsonFromBuffer(reqInfo);
	CheckIfJsonContains("roomId");
	JoinRoomRequest r;
	r.roomId = j["roomId"];
	return r;
}

SubmitAnswerRequest RequestPacketDeserializer::deserializeSubmitAnswerRequest(const RequestInfo& reqInfo)
{
	const json j = JsonFromBuffer(reqInfo);
	CheckIfJsonContains("answerId");
	SubmitAnswerRequest r;
	r.answerId = j["answerId"];
	return r;
}
