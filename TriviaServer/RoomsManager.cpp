#include "RoomsManager.h"


unsigned int getRoomId()
{
	static unsigned int currId = 0;
	return ++currId;
}


RoomsManager::RoomsManager(IDatabase* database)
	: database(database)
{
}

unsigned int RoomsManager::createRoom(const LoggedUser& admin, RoomData data)
{
	if (!data.answerTimeout) throw Exception("the answer timeout must be a positive number");
	if (!data.maxUsers) throw Exception("the number of players must be a positive number");
	if (!data.questionCount) throw Exception("the number of question must be a positive number");
	if (data.questionCount > database->getMaxQuestionsCount()) data.questionCount = database->getMaxQuestionsCount();

	data.id = getRoomId();
	data.hasGameStarted = false;
	data.isActive = true;
	rooms[data.id] = Room(data);
	rooms[data.id].addUser(admin);
	return data.id;
}

void RoomsManager::closeRoom(int roomId)
{
	checkIfExists(roomId);
	rooms[roomId].close();
}

void RoomsManager::deleteRoom(int roomId)
{
	checkIfExists(roomId);
	rooms.erase(roomId);
}

void RoomsManager::addUserToRoom(const LoggedUser& user, unsigned int roomId)
{
	checkIfExists(roomId);
	if (!rooms[roomId].getRoomData().hasGameStarted)
		rooms[roomId].addUser(user);
}

void RoomsManager::removeUserFromRoom(const LoggedUser& user, unsigned int roomId)
{
	checkIfExists(roomId);

	rooms[roomId].removeUser(user);
	if (rooms[roomId].isEmpty() && !rooms[roomId].isActive())
		deleteRoom(roomId);
}

void RoomsManager::startGame(unsigned int roomId)
{
	rooms[roomId].startGame();
}

vector<tuple<unsigned int, string>> RoomsManager::getRooms() const
{
	vector<tuple<unsigned int, string>> v;
	for (auto& i : rooms)
	{
		const RoomData& rd = i.second.getRoomData();
		if (!rd.hasGameStarted)
			v.push_back({ rd.id, rd.name });
	}
	return v;
}

Room& RoomsManager::getRoom(unsigned int roomId)
{
	checkIfExists(roomId);
	return rooms[roomId];
}

const RoomData& RoomsManager::getRoomData(unsigned int roomId) const
{
	checkIfExists(roomId);
	return rooms.at(roomId).getRoomData();
}

vector<string> RoomsManager::getUsernames(unsigned int roomId) const
{
	checkIfExists(roomId);
	return rooms.at(roomId).getUsernames();
}

vector<LoggedUser> RoomsManager::getUsers(unsigned int roomId) const
{
	checkIfExists(roomId);
	return rooms.at(roomId).getUsers();
}


void RoomsManager::checkIfExists(unsigned int roomId) const
{
	if (rooms.count(roomId) == 0)
		throw Exception("room does not exists. (id = " + to_string(roomId));
}
