#pragma once

#include "ofMain.h"
#include "GuiApp.h"
#include "ofxScreenSetup.h"
#include "ofxSerial.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void serialSetup();
        void buttonPressed(int button);

    // Basic Setup
    const int fps = 24;
    const bool singleMonitor = false;
    const int cameraQty = 3;
    const int cameraSizes[2] = { 960, 540 };
    const int cameraPositions[4][2] = { {0,0},{960,0},{0,540},{960,540} };
    const int cameraAssignments[3] = { 0, 0, 0 };
    const float cdTimeLength = 7000;
    const float cdFreezeLength = 2000;
    
    int deviceQty;
    const int deviceAssignments[3] = { 0, 1, 2 };
    
    // Fbo
    ofFbo fbo[3];
    int fboSizes[3][2];
    int fboPositions[3][2];

    // Screen & Interface Setup
    ofxScreenSetup screenSetup;
    shared_ptr<GuiApp> gui;
    
    // Screenshot
    ofImage screenshot[3];
    bool hasShot[3] = { false, false, false };
    
    // Video Recorder
    ofVideoGrabber vidGrabber[3];

    // Press Buttons
    ofx::IO::SerialDevice device[10];

    // Countdown
    bool isCountdown[3] = { false, false, false };
    float cdEndTime[3];
        
};
