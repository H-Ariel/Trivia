#pragma once

#include "RequestResult.h"


class IRequestHandler
{
public:
	virtual RequestResult handleRequest(const RequestInfo& reqInfo) = 0;
};
