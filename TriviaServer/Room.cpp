#include "Room.h"


Room::Room(const RoomData& data)
	: _data(data)
{
}

void Room::addUser(const LoggedUser& user)
{
	if (FIND_IN_ARRAY(_users, user))
		throw Exception("user already in room");
	if (_users.size() >= _data.maxUsers)
		throw Exception("This room is full");
	
	_users.push_back(user);
}

void Room::removeUser(const LoggedUser& user)
{
	auto it = find(_users.begin(), _users.end(), user);
	if (it == _users.end())
		throw Exception("user not in room");
	_users.erase(it);
}

vector<string> Room::getUsernames() const
{
	vector<string> usernames;
	for (auto& i : _users)
		usernames.push_back(i.getUsername());
	return usernames;
}

void Room::close()
{
	_data.isActive = false;
}

void Room::startGame()
{
	_data.hasGameStarted = true;
}
