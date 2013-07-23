#include "testApp.h"


/*
 Sets up a custom data class/object as a memory mapped file.
 - Can be a server (creates/destroys the data) or client (reads the data).
 - A memory key string "MyCustomData" is used to identify the shared memory (you can name change this to whatever you want).
 - The source code is identical for exampleDataClient and exampleDataServer, except for the variable below 'isServer'
 */


bool isServer = false;
string memoryKey = "MyCustomData";
int memorySize = sizeof(CustomData);

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofSetLogLevel(OF_LOG_VERBOSE);        

    // setup an object with different data types
    myCustomData = new CustomData();
    
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

	// server updates data and saves to memory mapped file
	// client loads memory mapped file into object
    if(isServer) {
        myCustomData->time = ofGetElapsedTimef();
        myCustomData->mouseX = ofGetMouseX();
        myCustomData->mouseY = ofGetMouseY();
        myCustomData->vec.x = sin(ofGetElapsedTimef()) * 200;
        myCustomData->vec.y = cos(ofGetElapsedTimef()*.5) * 200;
        myCustomData->vec.z = ofSignedNoise(ofGetElapsedTimef()*.25) * 200;
        strcpy(myCustomData->message, "Hello, I am a string message");
        
        memoryMappedFile.setData(myCustomData);
        
    } else {
        
		myCustomData = memoryMappedFile.getData();
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // server is blue, client is yellow
    (isServer) ? ofBackground(0, 255, 255) : ofBackground(255, 255, 0);
    
    // ofVec3f- pink cube outline
    ofNoFill();
    ofSetColor(255,0,255);
    ofBox(myCustomData->vec.x + (ofGetWidth()*.5), myCustomData->vec.y + (ofGetHeight()*.5), myCustomData->vec.z, 30);
    
    // mousex + mouseY- white circle
    ofFill();
    ofSetColor(255);
    ofCircle(myCustomData->mouseX , myCustomData->mouseY, 20);
    
    ofSetColor(255);
    stringstream output;
	output << "Shared data..." << endl
    << "Time : " << myCustomData->time << endl
    << "Mouse X : " << myCustomData->mouseX << endl
    << "Mouse Y : " << myCustomData->mouseY << endl
    << "Vec : " << myCustomData->vec << endl
    << "Message : " << myCustomData->message;
    ofDrawBitmapStringHighlight(output.str(), 20,20);
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