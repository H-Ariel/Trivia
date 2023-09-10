#pragma once

#include "IDatabase.h"


class StatisticsManager
{
public:
	StatisticsManager(IDatabase* database);

	vector<UserStatistics> getHighScore(); // returns the 3 best players.
	UserStatistics getUserStatistics(const string& username);

private:
	IDatabase* const _database;
};
