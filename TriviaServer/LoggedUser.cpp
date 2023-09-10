#include "LoggedUser.h"


LoggedUser::LoggedUser(int id, string username)
	: _username(username), _id(id)
{
}

bool LoggedUser::operator==(const LoggedUser& u) const
{
	return _username == u._username;
}

bool LoggedUser::operator==(int id) const
{
	return _id == id;
}

bool LoggedUser::operator<(const LoggedUser& u) const
{
	return _username < u._username;
}
