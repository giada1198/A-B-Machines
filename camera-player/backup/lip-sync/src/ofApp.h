#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"
#include "ofxThreadedImageLoader.h"

// listening port
#define PORT 6000

class ofApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
    
    void loadImages(ofDirectory& dir, vector<ofImage>& img, string name);

    string mode = "none";
    
    ofxOscReceiver oscReceiver;
    ofxSyphonServer syphonServer;
    
    ofxThreadedImageLoader threadedImageloader1;
    ofxThreadedImageLoader threadedImageloader2;
    ofxThreadedImageLoader threadedImageloader3;
    
    ofDirectory dirJackie;
    ofDirectory dirLiz;
    ofDirectory dirMarilyn;
    
    vector<ofImage> imgJackie;
    vector<ofImage> imgLiz;
    vector<ofImage> imgMarilyn;
    
    bool hasLoadedJackie = false;
    bool hasLoadedLiz = false;
    bool hasLoadedMarilyn = false;
    
    int currentImage = 0;
};
