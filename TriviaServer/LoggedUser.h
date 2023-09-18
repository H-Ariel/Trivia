#pragma once

#include "GeneralLibrary.h"


class LoggedUser
{
public:
	LoggedUser(void* key, string username);

	string getUsername() const { return _username; }
	void* getKey() const { return _key; }

	bool operator==(const LoggedUser&) const;
	bool operator==(void* key) const;
	bool operator<(const LoggedUser&) const;

private:
	string _username;
	void* _key; // this is an id for logged user
};
