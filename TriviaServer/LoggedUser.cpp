#include "LoggedUser.h"


LoggedUser::LoggedUser(SOCKET sock, string username)
	: _username(username), _sock(sock)
{
}

bool LoggedUser::operator==(const LoggedUser& u) const
{
	return _username == u._username;
}

bool LoggedUser::operator==(SOCKET sock) const
{
	return _sock == sock;
}

bool LoggedUser::operator<(const LoggedUser& u) const
{
	return _username < u._username;
}
