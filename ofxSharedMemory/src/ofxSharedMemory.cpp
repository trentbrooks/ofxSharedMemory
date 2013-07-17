#include "ofxSharedMemory.h"


ofxSharedMemory::ofxSharedMemory(){
        
    sharedData = NULL;
    isServer = false;
    memoryKey = "";
    memorySize = 0;
	isReady = false;
}


ofxSharedMemory::~ofxSharedMemory(){
    close();
}


void ofxSharedMemory::close() {
    
    if(isServer) {
		#ifdef _WIN32
			UnmapViewOfFile(sharedData);
			CloseHandle(hMapFile);
		#else
			munmap(sharedData, memorySize);
			shm_unlink(memoryKey.c_str());
		#endif
    }
    
}


void ofxSharedMemory::setup(string memoryKey, int memorySize, bool isServer) {
    
    this->memoryKey = memoryKey;
    this->memorySize = memorySize;
    this->isServer = isServer;

	#ifdef _WIN32
	tMemoryKey = std::wstring(memoryKey.begin(), memoryKey.end());
	#endif
}

bool ofxSharedMemory::connect() {
	#ifdef _WIN32

		if(isServer) {
			// server must use CreateFileMapping
			hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, memorySize, tMemoryKey.c_str());
		} else {
			hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, tMemoryKey.c_str());
		}
		if(hMapFile == NULL) {
			isReady = false;
			return false;
		}

		sharedData = (unsigned char*) MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS, 0, 0, memorySize);
		if(sharedData == NULL) {
			CloseHandle(hMapFile);
			isReady = false;
			return false;
		}
    #else 
    
        // create/connect to shared memory from dummy file
        int descriptor = shm_open(memoryKey.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if(descriptor == -1) {
			isReady = false;
			return false;
		}
        
        // map to memory
        ftruncate(descriptor, memorySize);
        sharedData = (unsigned char*) mmap(NULL, memorySize, PROT_WRITE | PROT_READ, MAP_SHARED, descriptor, 0);
        if(sharedData == NULL) {
			if(isServer) shm_unlink(memoryKey.c_str());
            isReady = false;
			return false;
		}
    
    #endif

    isReady = true;
    return true;
}

// copies source data to shared data
void ofxSharedMemory::setData(unsigned char * sourceData) {
    memcpy(sharedData, sourceData, memorySize);
}

// returns shared data
unsigned char * ofxSharedMemory::getData() {
    return sharedData;
}

