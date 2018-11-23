#pragma once

#define OPENFACE_USE_MULTI true

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "ofxOpenFace.h"

// listening port
#define PORT 6000

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
    bool bRunNewFrameOnly = false;
    
    ofFbo fbo;
    ofFbo fbo1, fbo2, fbo3;
    
    ofxSyphonClient mClient;
};
