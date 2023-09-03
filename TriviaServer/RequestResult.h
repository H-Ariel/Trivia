#pragma once

#include "GeneralLibrary.h"


class IRequestHandler;


struct RequestResult
{
	RequestInfo response;
	shared_ptr<IRequestHandler> newHandler;
	bool changeHandler = true;
};
