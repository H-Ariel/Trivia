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
	const vector<LoggedUser>& getUsers() const;

	const RoomData& getRoomData() const;
	void close(); // close the room
	bool isActive() const;
	bool isEmpty() const;
	void startGame();


private:
	RoomData data;
	vector<LoggedUser> users;
};
