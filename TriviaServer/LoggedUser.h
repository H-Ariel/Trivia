#pragma once

#include "GeneralLibrary.h"


class LoggedUser
{
public:
	LoggedUser(SOCKET sock, string username);

	string getUsername() const { return username; }
	SOCKET getSocket() const { return sock; }

	bool operator==(const LoggedUser&) const;
	bool operator==(SOCKET) const;
	bool operator<(const LoggedUser&) const;

private:
	string username;
	SOCKET sock; // this is kind of an id for logged user
};
