#include "Room.h"


Room::Room(RoomData data)
	: data(data)
{
}

void Room::addUser(const LoggedUser& user)
{
	if (FIND_IN_ARRAY(users, user)) throw Exception("user already in room");
	if (users.size() >= data.maxUsers) throw Exception("This room is full");
	users.push_back(user);
}

void Room::removeUser(const LoggedUser& user)
{
	auto it = find(users.begin(), users.end(), user);
	if (it == users.end())
		throw Exception("user not in room");
	users.erase(it);
}

vector<string> Room::getUsernames() const
{
	vector<string> users_str;
	for (auto& i : users)
		users_str.push_back(i.getUsername());
	return users_str;
}
const vector<LoggedUser>& Room::getUsers() const
{
	return users;
}

const RoomData& Room::getRoomData() const
{
	return data;
}

void Room::close()
{
	data.isActive = false;
}

bool Room::isActive() const
{
	return data.isActive;
}

bool Room::isEmpty() const
{
	return users.empty();
}

void Room::startGame()
{
	data.hasGameStarted = true;
}
