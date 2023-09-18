#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"


class LoginManager
{
public:
	LoginManager(IDatabase* database);

	LoggedUser login(void* key, string username, string password);
	LoggedUser signup(void* key, string username, string password, string email);
	void logout(const LoggedUser& user);
	void disconnectUser(void* key);

private:
	vector<LoggedUser> _loggedUsers;
	IDatabase* const _database;
};
