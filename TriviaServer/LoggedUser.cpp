#include "LoggedUser.h"


LoggedUser::LoggedUser(SOCKET sock, string username)
	: username(username), sock(sock)
{
}

bool LoggedUser::operator==(const LoggedUser& u) const
{
	return username == u.username;
}

bool LoggedUser::operator==(SOCKET sock) const
{
	return this->sock == sock;
}

bool LoggedUser::operator<(const LoggedUser& u) const
{
	return username < u.username;
}
