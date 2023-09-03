#pragma once

#include "GeneralLibrary.h"


class IRequestHandler;


struct RequestResult
{
	Buffer response;
	shared_ptr<IRequestHandler> newHandler;
	bool changeHandler = true;
};
