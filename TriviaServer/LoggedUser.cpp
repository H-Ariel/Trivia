#include "LoggedUser.h"


LoggedUser::LoggedUser(void* key, string username)
	: _username(username), _key(key)
{
}

bool LoggedUser::operator==(const LoggedUser& u) const
{
	return _username == u._username;
}

bool LoggedUser::operator==(void* key) const
{
	return _key == key;
}

bool LoggedUser::operator<(const LoggedUser& u) const
{
	return _username < u._username;
}
