#include "LoginManager.h"


LoginManager::LoginManager(IDatabase* database)
	: database(database)
{
}

const LoggedUser& LoginManager::login(SOCKET sock, string username, string password)
{
	LoggedUser newUser(sock, username);

	if (!database->doesUserExist(username)) throw Exception("username does not exists");
	if (FIND_IN_ARRAY(loggedUsers, newUser)) throw Exception("user already logged in");
	if (!database->doesPasswordMatch(username, password)) throw Exception("password does not match username");

	loggedUsers.push_back(newUser);
	return loggedUsers.back();
}

const LoggedUser& LoginManager::signup(SOCKET sock, string username, string password, string email)
{
	if (database->doesUserExist(username)) throw Exception("username already exists");

	database->addNewUser(username, password, email);
	return login(sock, username, password);
}

void LoginManager::logout(const LoggedUser& user)
{
	auto it = find(loggedUsers.begin(), loggedUsers.end(), user);
	if (it != loggedUsers.end())
	{
		loggedUsers.erase(it);
	}
}

void LoginManager::disconnectSocket(SOCKET sock)
{
	auto it = find(loggedUsers.begin(), loggedUsers.end(), sock);
	if (it != loggedUsers.end())
	{
		loggedUsers.erase(it);
	}
}
