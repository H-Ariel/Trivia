#pragma once

#include "LoggedUser.h"


class Room
{
public:
	Room() = default;
	Room(RoomData data);

	void addUser(const LoggedUser& user);
	void removeUser(const LoggedUser& user);
	vector<string> getUsernames() const;
	void close(); // close the room
	void startGame();

	const vector<LoggedUser>& getUsers() const { return _users; }
	const RoomData& getRoomData() const { return _data; }
	bool isActive() const { return _data.isActive; }
	bool isEmpty() const { return _users.empty(); }

private:
	RoomData _data;
	vector<LoggedUser> _users;
};
