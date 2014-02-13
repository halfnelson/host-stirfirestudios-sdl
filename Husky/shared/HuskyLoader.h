//
//  HuskyLoader.h
//  dummy
//
//  Created by Brendan Ragan on 20/12/13.
//  Copyright (c) 2013 Stirfire Studios. All rights reserved.
//

#ifndef __dummy__HuskyLoader__
#define __dummy__HuskyLoader__

#ifdef __APPLE__
	#include <dlfcn.h>
	typedef void* HuskyDLLHandle;
#elif WIN32
//	#include 
	#include <Windows.h>
	typedef HMODULE HuskyDLLHandle;
#endif

#include <iostream>
#include <string>
#include <map>

#include "Husky.h"

class HuskyLoaderHandle {
public:
	HuskyLoaderHandle();
	HuskyLoaderHandle(HuskyDLLHandle dllhandle);
	HuskyDLLHandle dllhandle;
};

typedef std::map<std::string, HuskyLoaderHandle> LoaderHandleMap;
//using LoaderHandleMap = std::map<std::string, HuskyLoaderHandle>;

class HuskyLoader {
public:
	static HuskyLoader *getInstance();
	unsigned long scanForHuskies();
	unsigned long scanForHuskies(char *directory);
	const char *getHuskyName(int index);
	Husky *getHuskyInstance(char *name);

private:
	static HuskyLoader* instance;
	
	HuskyLoader();
	~HuskyLoader();
	LoaderHandleMap *_map;
};

#endif /* defined(__dummy__HuskyLoader__) */
