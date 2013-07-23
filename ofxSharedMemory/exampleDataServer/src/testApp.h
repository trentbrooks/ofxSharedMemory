#pragma once

#include "ofMain.h"
#include "ofxSharedMemory.h"


class CustomData {
public:
	float time;
	int mouseX;
    int mouseY;
    ofVec3f vec;
    char message[256];
};


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // when creating an ofxSharedMemory, use the <> to assign a data type
    ofxSharedMemory<CustomData*> memoryMappedFile;
	CustomData* myCustomData;
    
    bool isConnected;
};
