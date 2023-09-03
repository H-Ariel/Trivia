#pragma once

#include "Room.h"
#include "LoggedUser.h"
#include "IDatabase.h"


class RoomsManager
{
public:
	RoomsManager(IDatabase* database);


	unsigned int createRoom(const LoggedUser& admin, RoomData data); // create room and returns its ID
	void closeRoom(int roomId);
	void addUserToRoom(const LoggedUser& user, unsigned int roomId);
	void removeUserFromRoom(const LoggedUser& user, unsigned int roomId);
	void startGame(unsigned int roomId);
	vector<tuple<unsigned int, string>> getRooms() const;
	Room& getRoom(unsigned int roomId);
	const RoomData& getRoomData(unsigned int roomId) const;
	vector<string> getUsernames(unsigned int roomId) const;
	vector<LoggedUser> getUsers(unsigned int roomId) const;

private:
	void deleteRoom(int id);
	void checkIfExists(unsigned int roomId) const;

	map<unsigned int, Room> rooms;
	IDatabase* const database;
};
