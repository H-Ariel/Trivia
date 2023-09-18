#include "LoginManager.h"


LoginManager::LoginManager(IDatabase* database)
	: _database(database)
{
}

LoggedUser LoginManager::login(void* key, string username, string password)
{
	LoggedUser newUser(key, username);

	if (!_database->doesUserExist(username))
		throw Exception("username does not exists");
	if (FIND_IN_ARRAY(_loggedUsers, newUser))
		throw Exception("user already logged in");
	if (!_database->doesPasswordMatch(username, password))
		throw Exception("password does not match username");

	_loggedUsers.push_back(newUser);
	return _loggedUsers.back();
}

LoggedUser LoginManager::signup(void* key, string username, string password, string email)
{
	if (_database->doesUserExist(username))
		throw Exception("username already exists");

	_database->addNewUser(username, password, email);
	return login(key, username, password);
}

void LoginManager::logout(const LoggedUser& user)
{
	auto it = find(_loggedUsers.begin(), _loggedUsers.end(), user);
	if (it != _loggedUsers.end())
		_loggedUsers.erase(it);
}

void LoginManager::disconnectUser(void* key)
{
	auto it = find(_loggedUsers.begin(), _loggedUsers.end(), key);
	if (it != _loggedUsers.end())
		_loggedUsers.erase(it);
}
