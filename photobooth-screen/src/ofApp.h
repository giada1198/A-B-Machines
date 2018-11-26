#pragma once

#include "ofMain.h"
#include "GuiApp.h"
#include "ofxScreenSetup.h"
#include "ofxSerial.h"

class ofApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();
		void exit();

	void buttonSetup();
    void buttonPressed(int button);
    static void virtualButtonPressed();
    string generateScreenShotName(int time, int number, bool isPhotoBooth);

	// Camera & Button Assignment
	const int cameraAssignments[3] = { 0, 2, 3 };
	const int serialDeviceAssignments[3] = { 5, 2, 3 };

    // basic setup
    const int fps = 24;
    const int photoBoothFps = 6;
    const int screenTestFps = 12;
    const bool singleMonitor = false;
    // string mode = "screen_test";
	string mode[3] = { "screen_test", "screen_test", "screen_test" };

    // camera
    const int cameraSizes[2] = { 960, 540 };
    const int cameraPositions[4][2] = { {0,0},{960,0},{0,540},{960,540} };

    // video grabber
    ofVideoGrabber vidGrabber[3];
    ofImage screenshot[3];
    bool hasShot[3] = { false, false, false };
    int screenShotQty[3];

    // screen test
    string screenTestStatus[3] = { "Standby", "Standby", "Standby" };
    ofImage screenTestGrid;
    const int screenTestGridSize[2] = { 540, 440 };
    const int screenTestGridDescent[3] = { 200, 200, 100 };  // Jackie, Liz, Marilyn

    // info box in screen test mode
    const int infoPosition[3][2] = { {540,0}, {540,960}, {0,0} };
    const string infoName[3] = { "Jackie", "Liz", "Marilyn" };

    // fbo
    ofFbo fbo[3];
    int fboSizes[3][2];
    int fboPositions[3][2];

    // gui & screen setup
    ofxScreenSetup screenSetup;
    shared_ptr<GuiApp> gui;
    ofImage numberImages[6];

    // serial
    vector<ofx::IO::SerialDevice> serialDevice;
    int serialDeviceQty;

    // countdown
    bool isCountdown[3] = { false, false, false };
    const float cdTimeLength = 7000;
    const float cdFreezeLength = 2000;
    float cdEndTime[3];
    float pressButtonTime[3];
    float cdNextScreenShotTime[3];
    int cdScreenShotNumber[3];

	// instagram
	ofImage instagramLayer[2];
};
