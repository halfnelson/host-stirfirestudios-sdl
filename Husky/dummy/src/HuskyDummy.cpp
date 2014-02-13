//
//  HuskyDummy.cpp
//  dummy
//
//  Created by Brendan Ragan on 19/12/13.
//  Copyright (c) 2013 Stirfire Studios. All rights reserved.
//

#include "HuskyDummy.h"
#include <random>
#include <string.h>

HuskyDummy* HuskyDummy::instance;

EXPORT
HuskyDummy::HuskyDummy() {
	printf("Booting up dummy husky\n");
	_observer = NULL;
}

EXPORT
HuskyDummy::~HuskyDummy() {
	printf("Shutting Down dummy husky\n");
}

EXPORT
HuskyDummy* HuskyDummy::getInstance() {
	if(!instance) {
		instance = new HuskyDummy();
	}
	return instance;
}

EXPORT
void HuskyDummy::shutdownInstance() {
	if(instance) {
		delete(instance);
	}
}

EXPORT
uint16_t HuskyDummy::getCapabilities() {
	return HuskyHasAchievements | HuskyHasCloudSaves | HuskyHasLeaderboards;
}

EXPORT
void HuskyDummy::setObserver(HuskyObserver *observer) {
	_observer = observer;
}

EXPORT
void HuskyDummy::setAchievement(const char *name) {
	if (_observer != NULL) {
		if (strcasecmp(name, "Failed Achievement") == 0) {
			std::cout << "Dummy Husky: Failed Achievement used, sending failure callback: " << std::endl;
			_observer->HuskyObserverAchievementCallback(name, false);
		} else {
			std::cout << "Dummy Husky: sending success callback for " << name << std::endl;
			_observer->HuskyObserverAchievementCallback(name, true);
		}
	} else {
		std::cout << "Dummy Husky: Setting Achievement Earned: " << name << std::endl;
	}
}

EXPORT
void HuskyDummy::doTick() {
}

EXPORT
void HuskyDummy::resetAchievements() {
	printf("Resetting Achievements");
}

EXPORT
void HuskyDummy::uploadLeaderboardScore(const char *name, int32_t score, HuskyLeaderboardScoreToKeep tokeep, int64_t extradata) {
	const char *keepstring, *extrastring;
	if (extradata == 0) {
		extrastring = "no extra data";
	} else {
		extrastring = "with extra data";
	}
	switch(tokeep) {
		case HuskyLeaderboardScoreToKeepBest:
			keepstring = "keeping the best score";
			break;
		case HuskyLeaderboardScoreToKeepUpdate:
			keepstring = "Updating existing score";
			break;
		default:
			keepstring = " Unknown condition";
	}
	
	std::cout << "Dummy Husky: Uploading score to leaderboard " << name << " - score " << score << " " << keepstring << " " << extrastring << std::endl;

	if (_observer == NULL)
		return;
	if (strcasecmp(name, "Failed Leaderboard") == 0) {
		std::cout << "Dummy Husky: Failed Leaderboard used, sending failure callback: " << std::endl;
		_observer->HuskyObserverLeaderboardScoreSetCallback(name, false);
	} else {
		std::cout << "Dummy Husky: sending success callback for Leaderboard " << name  << std::endl;
		_observer->HuskyObserverLeaderboardScoreSetCallback(name, true);
	}
}

HuskyLeaderboardEntry *generateEntries(int number, int startRank) {
	HuskyLeaderboardEntry *entries = (HuskyLeaderboardEntry*)calloc(sizeof(HuskyLeaderboardEntry), number);
	std::default_random_engine generator;
	std::string names[6] = {"Joe Bloggs", "Joe Schmoe", "John Winchester", "Frogman", "Salazar", "Greebo"};
	std::uniform_int_distribution<int> name_random(0,5);
	std::uniform_int_distribution<int> score_random(0,100000);
	for(int i = 0; i < number; i++) {
		int index = name_random(generator);
		std::string *aname = &names[index];
		entries[i].name = (char*)malloc((aname->length() + 1) * sizeof(char));
		strncpy((char*)entries[i].name, aname->c_str(), aname->length() + 1);
		entries[i].score = score_random(generator);
		entries[i].globalrank = startRank + i;
		entries[i].data = 0;
	}
	entries[0].name = "Joe Bloggs";
	entries[0].globalrank = 1;
	entries[0].score = 100;
	entries[0].data = 0;
	entries[1].name = "Joe Schmoe";
	entries[1].globalrank = 2;
	entries[1].score = 50;
	entries[1].data = 0;
	entries[2].name = "Joe Place";
	entries[2].globalrank = 3;
	entries[2].score = 10;
	entries[2].data = 0;
	return entries;
}

EXPORT
void HuskyDummy::requestLeaderboardScores(
										  const char *name, bool friends,
										  HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number) {
	const char *friendstr = "";
	if (friends) {
		printf(" Friends only ");
	}

	const char *timestr;
	
	switch(timeframe) {
		case HuskyLeaderboardTodaysScores:
			timestr = " from today";
			break;
		case HuskyLeaderboardWeeksScores:
			timestr = " from the last week";
			break;
		default:
			timestr = "";
	}
	
	std::cout << "Dummy Husky: Requesting " << offset << " scores starting at score " << number << " on leaderboard " << friendstr << timestr << std::endl;

	if (_observer) {
		if (strcasecmp(name, "Failed Leaderboard") == 0) {
			_observer->HuskyObserverLeaderboardScoreGetCallback(name, NULL, 0);
		} else {
			HuskyLeaderboardEntry *entries = generateEntries(number, offset);
			_observer->HuskyObserverLeaderboardScoreGetCallback(name, entries, 3);
		}
	}
}

EXPORT
void HuskyDummy::requestLeaderboardScoresNearPlayer(
										  const char *name, bool friends,
										  HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number) {
	const char *friendstr = "";
	if (friends) {
		printf(" Friends only ");
	}
	
	const char *timestr;
	
	switch(timeframe) {
		case HuskyLeaderboardTodaysScores:
			timestr = " from today";
			break;
		case HuskyLeaderboardWeeksScores:
			timestr = " from the last week";
			break;
		default:
			timestr = "";
	}
	
	std::cout << "Dummy Husky: Requesting " << number << " scores around player starting at score " << offset << " on leaderboard " << friendstr << timestr << std::endl;
	
	if (_observer) {
		if (strcasecmp(name, "Failed Leaderboard") == 0) {
			_observer->HuskyObserverLeaderboardScoreGetCallback(name, NULL, 0);
		} else {
			HuskyLeaderboardEntry *entries = generateEntries(number, offset);
			_observer->HuskyObserverLeaderboardScoreGetCallback(name, entries, 3);
		}
	}
}

EXPORT
void HuskyDummy::uploadCloudData(const char *cloudfilename, void *data, int32_t bytes) {
	std::cout << "Dummy Husky: Uploading data to cloud file: \"" << cloudfilename << "\" data is size: " << bytes << " Bytes" << std::endl;
	if (_observer) {
		if (strcasecmp(cloudfilename, "failure")) {
			_observer->HuskyObserverCloudDataUploaded(cloudfilename, false);
		} else {
			_observer->HuskyObserverCloudDataUploaded(cloudfilename, true);
		}
	}
}

EXPORT
void HuskyDummy::requestCloudData(const char *cloudfilename) {
	std::cout << "Dummy Husky: Requesting cloud file: \"" << cloudfilename << "\"" << std::endl;
	if (_observer) {
		if (strcasecmp(cloudfilename, "failure")) {
			_observer->HuskyObserverCloudDataDownloaded(cloudfilename, NULL, 0);
		} else {
			const char *data = "CLOUD DATA";
			_observer->HuskyObserverCloudDataDownloaded(cloudfilename, (void*)data, strlen(data) * sizeof(char));
		}
	}
}

EXPORT
Husky *getHuskyInstance() {
	return HuskyDummy::getInstance();
}

EXPORT
void shutdownHuskyInstance() {
	HuskyDummy::shutdownInstance();
}

EXPORT
char *getHuskyName() {
	return (char*)"Dummy";
}