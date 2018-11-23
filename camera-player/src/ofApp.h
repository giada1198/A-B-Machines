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
    
    bool loadImagesDir(vector<string>& dir, string name);
    void loadAudienceImages();

    inline string exec(const char* cmd);
    inline vector<string> split(const string& str, const string& delim);
    
    ofxOscReceiver oscReceiver;
    ofxSyphonServer syphonServer;
    ofxSyphonClient mClient;

    string mode = "lip_sync";
    
    // OpenFace
    ofxOpenFace::Thread oft;
    ofxOpenFace::Tracker tkr;
    bool isOpenFaceFirstRun = true;
    
    ofFbo fbo1, fbo2, fbo3, fboLips;
    
    vector<string> dirAudience[3];
    vector<ofImage> imgAudience[3];
    bool hasLoadedAudience = false;
    string imgAudienceDirName[3] = { "audience-1", "audience-2", "audience-3" };
    int currentImageAudience[4][3];
};
