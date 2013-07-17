#pragma once

#include "ofMain.h"
#include <stdio.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#else
#include <sys/shm.h>
#endif


/*
 ofxSharedMemory by Trent Brooks
 Openframeworks example of a memory mapped file for sharing data between multiple applications.
 - https://en.wikipedia.org/wiki/Memory-mapped_file
 
 Thanks to sloopi & KJ1 on the openframeworks forum for introducing me to 'memory mapped files'
 - http://forum.openframeworks.cc/index.php/topic,11730.msg55510.html#msg55510
 
 KJ1 created an excellent Windows/VS specific example for kinect (https://github.com/Kj1/ofxKinectMemoryMapped) which shares kinect data from a C# app with an OF app. I made this because I needed a pure C++ example that worked on osx and windows for sharing any kind of data.
 
 TODO::
 - need to allow reconnection for the client when server closes the shared memory without having to connect() in update/every frame
 - example for sharing non image data- eg. floats, ints, strings, etc.
 */


class ofxSharedMemory {
    
public:

    ofxSharedMemory();
    virtual ~ofxSharedMemory();
    
    void setup(string memoryKey, int size, bool isServer=true);
	bool connect();
    void close();

    void setData(unsigned char * sourceData);
    
    // data to share
    unsigned char * getData();


protected:

    bool isServer; // only the server can 'close' the shared data
    string memoryKey;
    int memorySize;
    
    unsigned char *sharedData;
	bool isReady;
  
	#ifdef _WIN32
	std::wstring tMemoryKey;
    HANDLE hMapFile;
	#endif
};

