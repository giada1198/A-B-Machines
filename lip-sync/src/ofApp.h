#pragma once

#define OPENFACE_USE_MULTI true

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "ofxOpenFace.h"

// listening port
#define PORT 9000

class ofApp : public ofBaseApp
{
    public:
        void setup();
        void update();
        void draw();
        void exit();    
        void keyPressed(int key);
    
    ofxOscReceiver oscReceiver;
    ofxSyphonServer syphonServer;
    
    ofxOpenFace::Thread oft;
    ofxOpenFace::Tracker tkr;
    ofVideoGrabber grabber;
    
    ofFbo fbo1, fbo2, fbo3, fbo4;
    ofFbo mask;
    
    bool isLipSync = true;
    ofxSyphonClient mClient;
};
