#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"


class LoginManager
{
public:
	LoginManager(IDatabase* database);

	const LoggedUser& login(SOCKET sock, string username, string password);
	const LoggedUser& signup(SOCKET sock, string username, string password, string email);
	void logout(const LoggedUser& user);
	void disconnectSocket(SOCKET sock);

private:
	vector<LoggedUser> _loggedUsers;
	IDatabase* const _database;
};
