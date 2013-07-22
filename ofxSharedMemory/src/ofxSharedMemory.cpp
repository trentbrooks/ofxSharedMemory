#include "ofxSharedMemory.h"

/*class SharedData {
public:
    
    char str[256];
    int x, y;
    float z;
    vector<ofVec3f> pts;
    
    size_t getSize() {
        return sizeof(str) + sizeof(x) + sizeof(y) + sizeof(z) + (sizeof(ofVec3f) * pts.size());
    }
};*/
template <typename T>
ofxSharedMemory<T>::ofxSharedMemory(){
        
    sharedData = NULL;
    isServer = false;
    memoryKey = "";
    memorySize = 0;
	isReady = false;
    
    //cout << sizeof(SharedData) << endl;
    //cout << sizeof(ofVec3f) << endl;
    
    
    // excellent - thanks Kyle
    /*SharedData sd;
    sd.x = 4321;
    sd.y = -980;
    sd.z = -3.457919;
    sd.pts.push_back(ofVec3f(1.1,2.2,3.3));
    sd.pts.push_back(ofVec3f(-1.1,-2.2,-3.3));
    sd.pts.push_back(ofVec3f(-3.1,-2.2,-1.3));
    string str = "hello,folks";
    strcpy(sd.str, str.c_str());
    
    //ofLog() << "sd: " << sizeof(sd);
    ofLog() << sd.pts.size();
    cout << sd.getSize() << endl;
    ofLog() << sizeof(sd.x);
    ofLog() << sizeof(sd.y);
    ofLog() << sizeof(sd.z);
    ofLog() << sizeof(sd.str);
    
    SharedData mem;
    memcpy(&mem, &sd, sd.getSize()); //sizeof(SharedData));
    //ofLog() << "sd2: " << sizeof(mem);
    ofLog() << mem.x;
    ofLog() << mem.y;
    ofLog() << mem.z;
    ofLog() << mem.str;
    ofLog() << mem.pts[2].z;
    ofLog() << "done";*/
}

/*template <typename T>
ofxSharedMemory<T>::~<T>ofxSharedMemory(){
    close();
}*/

template <typename T>
void ofxSharedMemory<T>::close() {
    
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

template <typename T>
void ofxSharedMemory<T>::setup(string memoryKey, int memorySize, bool isServer) {
    
    this->memoryKey = memoryKey;
    this->memorySize = memorySize;
    this->isServer = isServer;

	#ifdef _WIN32
	tMemoryKey = std::wstring(memoryKey.begin(), memoryKey.end());
	#endif
}

template <typename T>
bool ofxSharedMemory<T>::connect() {
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
    
		sharedData = (T) MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS, 0, 0, memorySize);
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
        sharedData = (T) mmap(NULL, memorySize, PROT_WRITE | PROT_READ, MAP_SHARED, descriptor, 0);
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
template <typename T>
void ofxSharedMemory<T>::setData(T sourceData) {
    memcpy(sharedData, sourceData, memorySize);
}

// returns shared data
template <typename T>
T ofxSharedMemory<T>::getData() {
    return sharedData;
}

