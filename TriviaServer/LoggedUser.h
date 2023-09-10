#pragma once

#include "GeneralLibrary.h"


class LoggedUser
{
public:
	LoggedUser(int id, string username);

	string getUsername() const { return _username; }
	SOCKET getSocket() const { return _id; }

	bool operator==(const LoggedUser&) const;
	bool operator==(int id) const;
	bool operator<(const LoggedUser&) const;

private:
	string _username;
	int _id; // this is an id for logged user
};
