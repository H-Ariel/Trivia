#pragma once

#include "GeneralLibrary.h"


class LoggedUser
{
public:
	LoggedUser(SOCKET sock, string username);

	string getUsername() const { return _username; }
	SOCKET getSocket() const { return _sock; }

	bool operator==(const LoggedUser&) const;
	bool operator==(SOCKET) const;
	bool operator<(const LoggedUser&) const;

private:
	string _username;
	SOCKET _sock; // this is kind of an id for logged user
};
