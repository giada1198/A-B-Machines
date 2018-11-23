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

    bool loadImagesDir(vector<string>& dir, string name);
    void loadAudienceImages();
    void playAudienceImages();
    
    inline string exec(const char* cmd);
    inline vector<string> split(const string& str, const string& delim);
    
    string mode = "none";

    ofxOscReceiver oscReceiver;
    ofxSyphonServer syphonServer;
    ofxSyphonServer syphonAudience[3];
    
    ofxThreadedImageLoader threadedImageloader[3];
    
    vector<string> dirJackie, dirLiz, dirMarilyn;
    int currentImage = 0;

    vector<string> dirAudience[3];
    vector<ofImage> imgAudience[3];
    bool hasLoadedAudience = false;
    string imgAudienceDirName[3] = { "audience-1", "audience-2", "audience-3" };
    int currentImageAudience[3][15];
};
