#pragma once

#include "GeneralLibrary.h"


class ResponsePacketSerializer
{
public:
	static Buffer serializeOkResponse();
	static Buffer serializeErrorResponse(const string& message);

	static Buffer serializeResponse(const GetRoomsResponse&);
	static Buffer serializeResponse(const GetStatisticsResponse&);

	static Buffer serializeResponse(const GetRoomStateResponse&);

	static Buffer serializeResponse(const GetQuestionResponse&);

	static Buffer serializeResponse(const GetGameResultsResponse&);
};


class RequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(const RequestInfo& reqInfo);
	static SignupRequest deserializeSignupRequest(const RequestInfo& reqInfo);

	static CreateRoomRequest deserializeCreateRoomRequest(const RequestInfo& reqInfo);
	static JoinRoomRequest deserializeJoinRoomRequest(const RequestInfo& reqInfo);

	static SubmitAnswerRequest deserializeSubmitAnswerRequest(const RequestInfo& reqInfo);
};
