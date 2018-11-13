#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GuiApp: public ofBaseApp
{
    public:
        void setup();
        void update();
        void draw();
    
    void updateLabel(int& val);
    void button1Pressed();
    void button2Pressed();
    void button3Pressed();
    
    ofxPanel gui;
    ofParameterGroup parameters;
    
    ofParameter<bool> isScreenTest;
    ofParameter<int> performance;
    ofParameter<string> performanceLabel;
    const string sessions[8] = { "Rehearsal",          "Tech Rehearsal",
                                 "November 28 8:00pm", "November 29 8:00pm",
                                 "November 30 4:00pm", "November 30 8:00pm",
                                 "December 01 4:00pm", "December 01 8:00pm" };
    
    ofxButton button[3];
    ofParameter<bool> isVirtualButtonPressed[3];
};
