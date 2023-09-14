#include "RoomsManager.h"


RoomsManager::RoomsManager(IDatabase* database)
	: _database(database)
{
}

int RoomsManager::createRoom(const LoggedUser& admin, RoomData data)
{
	if (data.answerTimeout <= 0)
		throw Exception("the answer timeout must be a positive number");
	if (data.maxUsers <= 0)
		throw Exception("the number of players must be a positive number");
	if (data.questionCount <= 0)
		throw Exception("the number of question must be a positive number");

	int maxQuestionsCount = _database->getMaxQuestionsCount();
	if (data.questionCount > maxQuestionsCount)
		data.questionCount = maxQuestionsCount;

	data.id = _database->getGameId();
	data.hasGameStarted = false;
	data.isActive = true;
	_rooms[data.id] = Room(data);
	_rooms[data.id].addUser(admin);
	return data.id;
}

void RoomsManager::closeRoom(int roomId)
{
	checkIfExists(roomId);
	_rooms[roomId].close();
}

void RoomsManager::deleteRoom(int roomId)
{
	checkIfExists(roomId);
	_rooms.erase(roomId);
}

void RoomsManager::addUserToRoom(const LoggedUser& user, int roomId)
{
	checkIfExists(roomId);
	if (!_rooms[roomId].getRoomData().hasGameStarted)
		_rooms[roomId].addUser(user);
}

void RoomsManager::removeUserFromRoom(const LoggedUser& user, int roomId)
{
	checkIfExists(roomId);

	_rooms[roomId].removeUser(user);
	if (_rooms[roomId].isEmpty() && !_rooms[roomId].isActive())
		deleteRoom(roomId);
}

void RoomsManager::startGame(int roomId)
{
	_rooms[roomId].startGame();
}

map<int, string> RoomsManager::getRooms() const
{
	map<int, string> v;
	for (auto& i : _rooms)
	{
		const RoomData& rd = i.second.getRoomData();
		if (!rd.hasGameStarted)
			v[rd.id] = rd.name;
	}
	return v;
}

Room& RoomsManager::getRoom(int roomId)
{
	checkIfExists(roomId);
	return _rooms[roomId];
}

const RoomData& RoomsManager::getRoomData(int roomId) const
{
	checkIfExists(roomId);
	return _rooms.at(roomId).getRoomData();
}

vector<string> RoomsManager::getUsernames(int roomId) const
{
	checkIfExists(roomId);
	return _rooms.at(roomId).getUsernames();
}

vector<LoggedUser> RoomsManager::getUsers(int roomId) const
{
	checkIfExists(roomId);
	return _rooms.at(roomId).getUsers();
}


void RoomsManager::checkIfExists(int roomId) const
{
	if (_rooms.count(roomId) == 0)
		throw Exception("room does not exists. (id = " + to_string(roomId));
}
