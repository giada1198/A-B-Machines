#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"

// osc listening port
#define PORT 6000

class GuiApp: public ofBaseApp
{
    public:
        void setup();
        void update();
        void draw();

    void button1Pressed();
    void button2Pressed();
    void button3Pressed();
    void resetButtonPressed();

    ofxOscReceiver oscReceiver;

    ofxPanel gui;
    ofParameterGroup parameters;
    ofParameter<bool> isScreenTest[3];

    ofxButton button[3];
    ofxButton resetButton;
    ofParameter<bool> isVirtualButtonPressed[3];
    ofParameter<bool> isButtonReseted;
};
