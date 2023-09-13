#pragma once

#include <string>


class Exception
{
public:
	inline Exception(const std::string& msg) : _msg(msg) {}
	inline const std::string& what() const { return _msg; }

private:
	std::string _msg;
};
