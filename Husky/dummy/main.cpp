//
//  main.cpp
//  dummyTest
//
//  Created by Brendan Ragan on 19/12/13.
//  Copyright (c) 2013 Stirfire Studios. All rights reserved.
//

#include <iostream>
#include "../shared/HuskyLoader.h"
#ifdef __APPLE__
	#include <inttypes.h>
#endif 

class TestObserver : public HuskyObserver {
	void HuskyObserverAchievementCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreSetCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreGetCallback(const char *name, HuskyLeaderboardEntry *entries, int number);
	void HuskyObserverCloudDataUploaded(const char *cloudfilename, bool success);
	void HuskyObserverCloudDataDownloaded(const char *cloudfilename, void *data, int32_t bytes);
};

void TestObserver::HuskyObserverAchievementCallback(const char *name, bool success) {
	if (success) {
		std::cout << "Achievement " << name << " set successfully!" << std::endl;
	} else {
		std::cout << "Achievement " << name << " could not be set :(" << std::endl;
	}
}

void TestObserver::HuskyObserverLeaderboardScoreSetCallback(const char *name, bool success) {
	if (success) {
		std::cout << "new score for Leaderboard " << name << " uploaded successfully!" << std::endl;
	} else {
		std::cout << "new score for Leaderboard " << name << " uploaded failed :(" << std::endl;
	}
}

void TestObserver::HuskyObserverLeaderboardScoreGetCallback(const char *name, HuskyLeaderboardEntry *entries, int number) {
	printf("Recieved scores for leaderboard: %s", name);
	for(int i = 0; i < number; i++) {
		std::cout << entries[i].globalrank << ". " << entries[i].name << " Score: " << entries[i].score;
		if (entries[i].data != 0)
			std::cout << " Data: " << entries[i].data;
		std::cout << std::endl;
	}
}

void TestObserver::HuskyObserverCloudDataUploaded(const char *cloudfilename, bool success) {
	if (success) {
		std::cout << " Uploaded Data to cloud file \"" << cloudfilename << "\"" << std::endl;
	} else {
		std::cout << " Failed to Uploaded Data to cloud file \"" << cloudfilename << "\"" << std::endl;
	}
}

void TestObserver::HuskyObserverCloudDataDownloaded(const char *cloudfilename, void *data, int32_t bytes) {
	if (bytes > 0) {
		std::cout << " Downloaded cloud file \"" << cloudfilename << "\" and got " << bytes << " data " << std::endl;
	} else {
		std::cout << " Failed to download any data for cloud file \"" << cloudfilename << "\"" << std::endl;
	}
}

int main(int argc, const char * argv[]) {
	HuskyLoader *loader = HuskyLoader::getInstance();
	unsigned long huskies = loader->scanForHuskies();
	std::cout << "Found " << huskies << " Huskies " << std::endl;
	for(int i = 0; i < huskies; i++) {
		std::cout << "Husky " << i << " is for: " << loader->getHuskyName(i) << std::endl;
	}
		
	Husky *husky = loader->getHuskyInstance((char*)"Dummy");

	std::cout << "Excercising Husky" << std::endl << std::endl;
	
	uint16_t capabilities = husky->getCapabilities();
	if (capabilities && HuskyHasAchievements) {
		std::cout << "#####################" << std::endl;
		std::cout << "ACHIEVEMENTS" << std::endl;
		std::cout << "#####################" << std::endl;

		std::cout << "Trying to set an achievement" << std::endl;
		husky->setAchievement((char*)"Test Achievement");
		std::cout << "Trying to set an achievement with an observer." << std::endl;
		TestObserver* observer = new TestObserver();
		husky->setObserver(observer);
		husky->setAchievement((char*)"Test Achievement");
		husky->setAchievement((char*)"Failed Achievement");
		std::cout << "Observer should have generated output above" << std::endl;

		std::cout << std::endl;
		std::cout << std::endl;
	}
	
	if (capabilities && HuskyHasLeaderboards) {
	
		std::cout << "#####################" << std::endl;
		std::cout << "LEADERBOARDS" << std::endl;
		std::cout << "#####################" << std::endl;
		
		std::cout << "Trying to upload a score of 100 to \"Leaderboard 1\" which keeps the best score" << std::endl;
		husky->uploadLeaderboardScore("Leaderboard 1", 100, HuskyLeaderboardScoreToKeepBest, 0);
		std::cout << "Trying to upload a score of 200 to \"Leaderboard 2\" which keeps the newest score" << std::endl;
		husky->uploadLeaderboardScore("Leaderboard 1", 100, HuskyLeaderboardScoreToKeepUpdate, 0);
		
		std::cout << "Trying to upload a score of 100 to \"Failed Leaderboard\" which should fail" << std::endl;
		husky->uploadLeaderboardScore("Failed Leaderboard", 100, HuskyLeaderboardScoreToKeepBest, 0);

		std::cout << "Trying to retrieve top 10 scores from \"Failed Leaderboard\" (which should have no scores)" << std::endl;
		husky->requestLeaderboardScores("Failed Leaderboard", false, HuskyLeaderboardAllScores, 0, 10);
		
		std::cout << "Trying to retrieve top 10 scores from \"Leaderboard 1\"" << std::endl;
		husky->requestLeaderboardScores("Leaderboard 1", false, HuskyLeaderboardAllScores, 0, 10);

		std::cout << "Trying to retrieve top 10 scores from \"Leaderboard 1\" among user's friends" << std::endl;
		husky->requestLeaderboardScores("Leaderboard 1", true, HuskyLeaderboardAllScores, 0, 10);
		
		std::cout << "Trying to retrieve top 10 scores from \"Leaderboard 1\" among user's friends in the last week" << std::endl;
		husky->requestLeaderboardScores("Leaderboard 1", true, HuskyLeaderboardWeeksScores, 0, 10);

		std::cout << std::endl;
		std::cout << std::endl;
	}

	if (capabilities && HuskyHasCloudSaves) {
		std::cout << "#####################" << std::endl;
		std::cout << "Cloud Storage" << std::endl;
		std::cout << "#####################" << std::endl;
		
		FILE *handle = NULL;
//		handle = fopen("/tmp/cloudtest.txt", "w");
		if (handle) {
			const char *teststring = "TEST CLOUD FILE";
			std::cout << "Trying to upload the cloud test file" << std::endl;
			husky->uploadCloudData("test", (void*)teststring, strlen(teststring));
			std::cout << "Trying to upload the failing test file" << std::endl;
			husky->uploadCloudData("failure", (void*)teststring, strlen(teststring));

			std::cout << "Trying to download a cloud test file" << std::endl;
			husky->requestCloudData("cloudtest.txt");
			std::cout << "Trying to download a failing test file" << std::endl;
			husky->requestCloudData("failure");
		}
	}
	getchar();
	return 0;
}
