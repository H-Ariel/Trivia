#pragma once

#include "GeneralLibrary.h"


class IRequestHandler;


struct RequestResult
{
	RequestResult() = default;
	RequestResult(const RequestInfo& response)
		: response(response), changeHandler(false) {}
	RequestResult(const RequestInfo& response, shared_ptr<IRequestHandler> newHandler)
		: response(response), newHandler(newHandler), changeHandler(true) {}

	RequestInfo response;
	shared_ptr<IRequestHandler> newHandler;
	bool changeHandler = true;
};
