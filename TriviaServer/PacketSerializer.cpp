#include "PacketSerializer.h"
#include "json.hpp" // todo: use my json ?

using nlohmann::json;


json JsonFromBuffer(const RequestInfo& reqInfo)
{
	return json::parse(reqInfo.msgData);
}

Buffer BufferFromJson(const json& j, MessageCodes code = MessageCodes::OK)
{
	const string data(j.dump());
	const uint32_t dataLen = (uint32_t)data.length();
	byte b_dataLen[sizeof(dataLen)] = {};
	memcpy(b_dataLen, &dataLen, sizeof(dataLen));

	Buffer buf;
	buf.push_back((byte)code);
	for (uint32_t i = 0; i < sizeof(dataLen); buf.push_back(b_dataLen[i++]));
	for (const char& c : data) buf.push_back((byte)c);

	return buf;
}


Buffer ResponsePacketSerializer::serializeOkResponse()
{
	Buffer buf;
	buf.push_back(MessageCodes::OK);
	for (uint32_t i = 0; i < sizeof(uint32_t); i++) buf.push_back(0);
	return buf;
}

Buffer ResponsePacketSerializer::serializeErrorResponse(const string& message)
{
	return BufferFromJson({ {"message", message} }, MessageCodes::Error);
}


Buffer ResponsePacketSerializer::serializeResponse(const GetRoomsResponse& resp)
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

Buffer ResponsePacketSerializer::serializeResponse(const GetStatisticsResponse& resp)
{
	return BufferFromJson({ { "statistics", resp.statistics } });
}

Buffer ResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& resp)
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

Buffer ResponsePacketSerializer::serializeResponse(const GetQuestionResponse& resp)
{
	return BufferFromJson({
		{ "answers", resp.answers },
		{ "question", resp.question }
		});
}

Buffer ResponsePacketSerializer::serializeResponse(const GetGameResultsResponse& resp)
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
