#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"


class LoginManager
{
public:
	LoginManager(IDatabase* database);

	const LoggedUser& login(int id, string username, string password);
	const LoggedUser& signup(int id, string username, string password, string email);
	void logout(const LoggedUser& user);
	void disconnectUser(int id);

private:
	vector<LoggedUser> _loggedUsers;
	IDatabase* const _database;
};
