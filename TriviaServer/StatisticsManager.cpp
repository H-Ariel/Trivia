#include "StatisticsManager.h"


StatisticsManager::StatisticsManager(IDatabase* database)
	: _database(database)
{
}

vector<UserStatistics> StatisticsManager::getHighScore()
{
	return _database->getHighScore(3); // return top 3 users
}

UserStatistics StatisticsManager::getUserStatistics(const string& username)
{
	return _database->getUserStatistics(username);
}
