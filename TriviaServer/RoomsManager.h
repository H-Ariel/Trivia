#pragma once

#include "Room.h"
#include "LoggedUser.h"
#include "IDatabase.h"


class RoomsManager
{
public:
	RoomsManager(IDatabase* database);

	int createRoom(const LoggedUser& admin, RoomData data); // create room and returns its ID
	void closeRoom(int roomId);
	void addUserToRoom(const LoggedUser& user, int roomId);
	void removeUserFromRoom(const LoggedUser& user, int roomId);
	void startGame(int roomId);
	map<int, string> getRooms() const;
	Room& getRoom(int roomId);
	const RoomData& getRoomData(int roomId) const;
	vector<string> getUsernames(int roomId) const;
	vector<LoggedUser> getUsers(int roomId) const;

private:
	void deleteRoom(int id);
	void checkIfExists(int roomId) const;

	map<int, Room> _rooms;
	IDatabase* const _database;
};
