#pragma once

#include "ofMain.h"
#include "GuiApp.h"
#include "ofxScreenSetup.h"
#include "ofxVideoRecorder.h"
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
		
		shared_ptr<GuiApp> gui;

        const bool singleCamera = true;
        int cameraAmount;
        const int cameraPositions[3][2] = { {0,0},{0,544},{960,544} };

        // Screen Setup
        ofxScreenSetup screenSetup;
    
        // Video Recorder
        ofVideoGrabber      vidGrabber[4];
        ofxVideoRecorder    vidRecorder;
        bool bRecording;
        int sampleRate;
        int channels;
        string fileName;
        string fileExt;

        void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);

    // Press Buttons
    const int deviceQty = 1;
    const int deviceNumber[3] = { 0, 1, 2 };
    ofx::IO::SerialDevice device[10];
    
//        ofFbo recordFbo;
//        ofPixels recordPixels;
    
};
