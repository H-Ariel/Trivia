#include "LoginManager.h"


LoginManager::LoginManager(IDatabase* database)
	: _database(database)
{
}

const LoggedUser& LoginManager::login(int id, string username, string password)
{
	LoggedUser newUser(id, username);

	if (!_database->doesUserExist(username))
		throw Exception("username does not exists");
	if (FIND_IN_ARRAY(_loggedUsers, newUser))
		throw Exception("user already logged in");
	if (!_database->doesPasswordMatch(username, password))
		throw Exception("password does not match username");

	_loggedUsers.push_back(newUser);
	return _loggedUsers.back();
}

const LoggedUser& LoginManager::signup(int id, string username, string password, string email)
{
	if (_database->doesUserExist(username))
		throw Exception("username already exists");

	_database->addNewUser(username, password, email);
	return login(id, username, password);
}

void LoginManager::logout(const LoggedUser& user)
{
	auto it = find(_loggedUsers.begin(), _loggedUsers.end(), user);
	if (it != _loggedUsers.end())
		_loggedUsers.erase(it);
}

void LoginManager::disconnectUser(int id)
{
	auto it = find(_loggedUsers.begin(), _loggedUsers.end(), id);
	if (it != _loggedUsers.end())
		_loggedUsers.erase(it);
}
