#include "testApp.h"


/*
 Sets up a 640x480 rgb video/camera input image as a memory mapped file.
 - Can be a server (creates/destroys the video) or client (reads the video).
 - A memory key string "MyVideo640x480" is used to identify the shared memory (you can name change this to whatever you want).
 - The source code is identical for exampleVideoClient and exampleVideoServer, except for the variable below 'isServer'
 */


bool isServer = true;
int videoWidth = 640;
int videoHeight = 480;
int memorySize = videoWidth * videoHeight * 3;
string memoryKey = "MyVideo640x480";


//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofSetLogLevel(OF_LOG_VERBOSE);
        
    if(isServer) {
        //video.setDeviceID(1);
        video.initGrabber(videoWidth,videoHeight);
    }

    texture.allocate(videoWidth,videoHeight, GL_RGB);

	// setup + connect
	memoryMappedFile.setup(memoryKey, memorySize, isServer);
    isConnected = memoryMappedFile.connect();
    
    ofLog() << "Memory was mapped? " << isConnected;
    ofLog() << "Memory key: " << memoryKey;
    ofLog() << "Memory size: " << memorySize;
}



//--------------------------------------------------------------
void testApp::update(){
    
    ofSetWindowTitle( ofToString(isServer ? "Server connected: " : "Client connected: ") + ofToString(isConnected ? "YES" : "NO") + ", FPS: " + ofToString(ofGetFrameRate()));
	
	// if not connected, try reconnect every 5 seconds or so
	if(!isConnected && ofGetFrameNum() % 300 == 0) {
		isConnected = memoryMappedFile.connect();
	}

	// server updates video and saves to memory mapped file
	// client loads memory mapped file into texture
    if(isServer) {

        video.update();
        if(video.isFrameNew()) {
            
            // copy pixel data from video into the memory mapped file buffer
            if(isConnected) memoryMappedFile.setData(video.getPixels());
            
            // not required for server, but loading the memory mapped file data back into a texture for sanity check
            //texture.loadData(memoryMappedFile.getData(), videoWidth, videoHeight, GL_RGB);

            texture.loadData(video.getPixels(), videoWidth, videoHeight, GL_RGB);
        }
    } else {
        
		if(isConnected) {
			texture.loadData(memoryMappedFile.getData(), videoWidth, videoHeight, GL_RGB);
		}
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // server is blue, client is yellow
    (isServer) ? ofBackground(0, 255, 255) : ofBackground(255, 255, 0);
    
    ofSetColor(255);
    texture.draw(0,0);
    ofSetColor(0);    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}