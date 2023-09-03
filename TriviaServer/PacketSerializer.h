#pragma once

#include "GeneralLibrary.h"


class ResponsePacketSerializer
{
public:
	static RequestInfo serializeOkResponse();
	static RequestInfo serializeErrorResponse(const string& message);

	static RequestInfo serializeResponse(const GetRoomsResponse&);
	static RequestInfo serializeResponse(const GetStatisticsResponse&);
	static RequestInfo serializeResponse(const GetRoomStateResponse&);
	static RequestInfo serializeResponse(const GetQuestionResponse&);
	static RequestInfo serializeResponse(const GetGameResultsResponse&);
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
