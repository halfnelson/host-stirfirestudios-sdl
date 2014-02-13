//
//  HuskyLoader.cpp
//  dummy
//
//  Created by Brendan Ragan on 20/12/13.
//  Copyright (c) 2013 Stirfire Studios. All rights reserved.
//

#include "HuskyLoader.h"

#ifdef __APPLE__
	#include <dirent.h>
#elif WIN32
	#include <Shlwapi.h>
	#pragma comment(lib, "Shlwapi.lib")
#endif
#include <iterator>

HuskyLoader* HuskyLoader::instance;

HuskyLoaderHandle::HuskyLoaderHandle() {
	this->dllhandle = NULL;
}

HuskyLoaderHandle::HuskyLoaderHandle(HuskyDLLHandle dllhandle) {
	this->dllhandle = dllhandle;
}

bool endsWith(const char *string, const char *ending) {
	unsigned long length1, length2;
	length1 = strlen(string);
	length2 = strlen(ending);
	const char *endofstring = string + (length1 - length2);
	return strcmp(ending, endofstring) == 0;
}


HuskyLoader::HuskyLoader() {
	_map = new LoaderHandleMap();
}

unsigned long HuskyLoader::scanForHuskies() {
	return scanForHuskies(NULL);
}

unsigned long HuskyLoader::scanForHuskies(char *directory) {

#ifdef __APPLE__
	const char* extension = ".dylib";

	DIR *dir;
	if (directory == NULL) {
		dir = opendir(".");
	} else {
		dir = opendir(directory);
	}
	
	if (dir == NULL) {
		printf("Could not open specified directory!");
		return 0;
	}
	
	struct dirent *ent;
	while((ent = readdir(dir)) != NULL) {
		if (ent->d_type == DT_REG) {
			if (endsWith(ent->d_name, extension)) {
				void* dll_handle = dlopen(ent->d_name, RTLD_LOCAL|RTLD_LAZY);
				if (dll_handle) {
					HuskyGetStaticInstance* fHuskyInstance = (HuskyGetStaticInstance*)dlsym(dll_handle, "getHuskyInstance");
					HuskyGetName* fHuskyName = (HuskyGetName*)dlsym(dll_handle, "getHuskyName");
					if (fHuskyName && fHuskyInstance) {
						HuskyLoaderHandle *handleObj = new HuskyLoaderHandle(dll_handle);
						std::string *name = new std::string(fHuskyName());
						_map->insert(LoaderHandleMap::value_type(*name, *handleObj));
					}
				}
			}
		}
	}
#elif WIN32
	LPTSTR extension = "\*.dll";

	LPTSTR search;

	if (directory == NULL) {
		DWORD length = 255;
		search = (LPTSTR)malloc(sizeof(char) * length);
		DWORD result = GetModuleFileName(NULL, search, length - 1);
		if (result == 0) {
			strcpy(search, ".");
		}
		PathRemoveFileSpec(search);
	} else {
		search = (LPTSTR)malloc(sizeof(char) * (strlen(directory) + strlen(extension) + 1));
		strcpy(search, directory);
	}

	PathAppend(search, extension);

	WIN32_FIND_DATA findfiledata;
	HANDLE hFind = FindFirstFile(search, &findfiledata);
	if (hFind != INVALID_HANDLE_VALUE ) {
		do {
			HMODULE dll_handle = LoadLibrary(findfiledata.cFileName);
			if (dll_handle) {
				HuskyGetStaticInstance* fHuskyInstance = (HuskyGetStaticInstance*)GetProcAddress(dll_handle, "getHuskyInstance");
				HuskyGetName* fHuskyName = (HuskyGetName*)GetProcAddress(dll_handle, "getHuskyName");
				if (fHuskyName && fHuskyInstance) {
					HuskyLoaderHandle *handleObj = new HuskyLoaderHandle(dll_handle);
					std::string *name = new std::string(fHuskyName());
					_map->insert(LoaderHandleMap::value_type(*name, *handleObj));
				}
			}
		} while(FindNextFile(hFind, &findfiledata) != 0);
		FindClose(hFind);
	}

	free(search);
#endif
	return _map->size();
}

const char* HuskyLoader::getHuskyName(int index) {
	if (index > _map->size())
		return NULL;

	LoaderHandleMap::iterator i = _map->begin();
	std::advance(i, index);
	return i->first.c_str();
}

Husky* HuskyLoader::getHuskyInstance(char *name) {
	std::string *strname = new std::string(name);
	HuskyLoaderHandle handleObj;
	try {
		handleObj = _map->at(*strname);
	} catch (std::exception &e) {
		return NULL;
	}
#ifdef __APPLE__
	HuskyGetStaticInstance* fHuskyInstance = (HuskyGetStaticInstance*)dlsym(handleObj.dllhandle, "getHuskyInstance");
#elif WIN32
	HuskyGetStaticInstance* fHuskyInstance = (HuskyGetStaticInstance*)GetProcAddress(handleObj.dllhandle, "getHuskyInstance");
#endif
	return fHuskyInstance();
}

HuskyLoader::~HuskyLoader() {
	free(_map);
}

HuskyLoader* HuskyLoader::getInstance() {
	if(!instance) {
		instance = new HuskyLoader();
	}
	return instance;
}