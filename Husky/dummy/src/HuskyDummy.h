//
//  HuskyDummy.h
//  dummy
//
//  Created by Brendan Ragan on 19/12/13.
//  Copyright (c) 2013 Stirfire Studios. All rights reserved.
//

#ifndef __dummy__HuskyDummy__
#define __dummy__HuskyDummy__

#include <iostream>
#include "Husky.h"

class
#ifdef WIN32
EXPORT
#endif
HuskyDummy : public Husky {
public:
	/** Get a handle to the husky singleton **/
	static HuskyDummy* getInstance();

	static void shutdownInstance();

	void setObserver(HuskyObserver* observer);

	uint16_t getCapabilities();

	void setAchievement(const char* name);

	void doTick();
	
	void resetAchievements();
	
	void uploadLeaderboardScore(const char *name, int32_t score, HuskyLeaderboardScoreToKeep tokeep, int64_t extradata);
	
	void requestLeaderboardScores(const char *name, bool friends, HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number);
	
	void requestLeaderboardScoresNearPlayer(const char *name, bool friends, HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number);

	void uploadCloudData(const char *cloudfilename, void* data, int32_t bytes);

	void requestCloudData(const char *cloudfilename);
private:
	static HuskyDummy* instance;
	
	HuskyDummy();
	~HuskyDummy();
	
	HuskyObserver* _observer;
	
};

#endif /* defined(__dummy__HuskyDummy__) */
