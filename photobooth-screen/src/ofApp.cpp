#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(255);
    ofSetFrameRate(fps);
    if(!singleMonitor)
    {
        screenSetup.setup(ofGetWidth(), ofGetHeight(), ofxScreenSetup::MONITOR_2);
    }
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
    buttonSetup();

    vidGrabber[0].listDevices();
    for (int i = 0; i < 3 ; i++)
    {
        vidGrabber[i].setDeviceID(cameraAssignments[i]);
        vidGrabber[i].initGrabber(cameraSizes[0],cameraSizes[1]);
        // Fbo Setting
        fbo[i].allocate(cameraSizes[0],cameraSizes[1]);
        fboSizes[i][0] = cameraSizes[0];
        fboSizes[i][1] = int(vidGrabber[i].getHeight()*(float(cameraSizes[0])/float(vidGrabber[i].getWidth())));
        fboPositions[i][0] = fboSizes[i][0];
        fboPositions[i][1] = -(fboSizes[i][1]-fboSizes[i][1])*0.5;
    }
    // load images
    screenTestGrid.load("screen-test-grid.png");
    for (int i = 0; i < 6; i++)
    {
        numberImages[i].load("number-" + to_string(i) + ".png");
    }
    // sync booth mode
    for (int k = 0; k < 3; k++)
    {
        if (gui->isScreenTest[k] == true) { mode[k] = "screen_test"; }
        else { mode[k] = "photo_booth"; }
    }
}

//--------------------------------------------------------------
void ofApp::buttonSetup()
{
    serialDevice.clear();
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    if (!devicesInfo.empty())
    {
        serialDeviceQty = devicesInfo.size();
        // allocate the vector to have as many SerialDevice as devicesInfo
        serialDevice.assign(serialDeviceQty, ofx::IO::SerialDevice());
        // connection info
        ofLogNotice("ofxSerial") << "Connected Devices: ";
        for(int i = 0; i < serialDeviceQty; i++)
        {
            ofLogNotice("ofxSerial") << devicesInfo[i];
            bool success = serialDevice[i].setup(devicesInfo[i], 9600);
            if(success)
            {
                ofLogNotice("ofxSerial") << "Successfully setup " << devicesInfo[i];
            }
            else
            {
                ofLogNotice("ofxSerial") << "Unable to setup " << devicesInfo[i];
            }
        }
    }
    else
    {
        ofLogNotice("ofxSerial") << "No devices connected.";
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    for(int i = 0; i < 3 ; i++)
    {
        // press button event from gui
        if (gui->isVirtualButtonPressed[i])
        {
            buttonPressed(i);
            gui->isVirtualButtonPressed[i] = false;
        }
        // scale and crop image from camera
        vidGrabber[i].update();
        fbo[i].begin();
        vidGrabber[i].draw( fboPositions[i][0], fboPositions[i][1],
                           -fboSizes[i][0],     fboSizes[i][1]);
        fbo[i].end();
    }
    // press reset button event
    if (gui->isButtonReseted)
    {
        buttonSetup();
        gui->isButtonReseted = false;
    }
    // receive serial command from arduino
    try
    {
        unsigned char buffer[1024]; // read all bytes from the device;
        for (int i = 0; i < serialDeviceQty; i++)
        {
            while (serialDevice[i].available() > 0)
            {
                string str;
                int sz = serialDevice[i].readBytes(buffer, 1024);
                for (int i = 0; i < sz ; ++i)
                {
                    str += buffer[i];
                }
                ofLogNotice("ofxSerial") << str;
                for (int k = 0; k < 3; k++)
                {
                    if (str == to_string(serialDeviceAssignments[k]) + "_press")
                    {
                        buttonPressed(k);
                    }
                }
            }
        }
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofApp::update") << exc.what();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0,0,0);
    ofSetColor(255,255,255);

    for (int k = 0; k < 3; k++)
    {
        // photo booth -> screen test
        if (gui->isScreenTest[k] && mode[k] == "photo_booth")
        {
            mode[k] = "screen_test";
            // reset all three booths
            isCountdown[k] = false;
            hasShot[k] = false;
        }
        // screen test -> photo booth
        else if (!gui->isScreenTest[k] && mode[k] == "screen_test")
        {
            mode[k] = "photo_booth";
            isCountdown[k] = false;
        }
    }

    // draw and save camera / screen shot
    for (int i = 0; i < 3; i++) {
        fbo[i].draw(cameraPositions[i][0], cameraPositions[i][1]);
        if(hasShot[i] && isCountdown[i])
        {
            screenshot[i].draw(cameraPositions[i][0], cameraPositions[i][1]);
        }
        // draw countdown image
        if(isCountdown[i])
        {
            float x = cdEndTime[i] - ofGetElapsedTimeMillis();  // time remaining on record
            float y = cdNextScreenShotTime[i] - ofGetElapsedTimeMillis();  // time remaining to the next shot
            // take screen shot and calculate time of next shot
            if(y < 0 && cdScreenShotNumber[i] < screenShotQty[i])
            {
                string path;
                if (mode[i] == "photo_booth")
                {
                    cdNextScreenShotTime[i] += 1000/photoBoothFps;
                    screenshot[i].grabScreen(cameraPositions[i][0], cameraPositions[i][1],
                                             cameraSizes[0], cameraSizes[1]);
                    path = "copy/audience-" + to_string(i+1) + "/" + generateScreenShotName(pressButtonTime[i], cdScreenShotNumber[i], true);
                }
                else if (mode[i] == "screen_test")
                {
                    cdNextScreenShotTime[i] += 1000/screenTestFps;
                    screenshot[i].grabScreen(cameraPositions[i][0]+screenTestGridDescent[i], cameraPositions[i][1],
                                             screenTestGridSize[1], screenTestGridSize[0]);
                    path = "copy/" + infoName[i] + "/" + generateScreenShotName(pressButtonTime[i], cdScreenShotNumber[i], false);
                }
                screenshot[i].save(path);
                cdScreenShotNumber[i] += 1;
            }
            // draw countdown image in photo booth mode
            if (mode[i] == "photo_booth")
            {
                ofEnableAlphaBlending();
                if(x >= cdTimeLength - 1000)  // draw "get ready" image
                {
                    numberImages[5].draw(cameraPositions[i][0]+280, cameraPositions[i][1],
                                         numberImages[5].getWidth(), numberImages[5].getHeight());
                }
                else if (x > 1000)  // draw image of number
                {
                    int m = int(x/1000)-1;
                    numberImages[m].draw(cameraPositions[i][0]+280, cameraPositions[i][1],
                                         numberImages[m].getWidth(), numberImages[m].getHeight());
                }
                else if (x >= 0)  // shot flash
                {
                    if(!hasShot[i])
                    {
                        screenshot[i].grabScreen(cameraPositions[i][0], cameraPositions[i][1],
                                                 cameraSizes[0], cameraSizes[1]);
                        string path = "copy/instagram/" + generateScreenShotName(pressButtonTime[i], 30, true);
                        screenshot[i].save(path);
                        hasShot[i] = true;
                    }
                    ofSetColor(255,255,255,(int(255*(1-abs(500-x)/500))));
                    ofDrawRectangle(cameraPositions[i][0], cameraPositions[i][1],
                                    cameraPositions[i][0] + cameraSizes[0],
                                    cameraPositions[i][1] + cameraSizes[1]);
                    ofSetColor(255);
                }
                else if (x <= -cdFreezeLength)  // after shot
                {
                    isCountdown[i] = false;
                    hasShot[i] = false;
                }
                ofDisableAlphaBlending();
            }
            else if (mode[i] == "screen_test")
            {
                if (x >= 0)
                {
                    screenTestStatus[i] = "Recording...";
                    // screenTestStatus[i] = "Recording... " + to_string(int(x/1000)+1) + "s left";
                }
                else
                {
                    isCountdown[i] = false;
                    screenTestStatus[i] = "Standby";
                }
            }
        }
        // draw instructions for actors in screen test mode
        if (mode[i] == "screen_test")
        {
            ofPushMatrix();
            ofTranslate(0,1080,0);
            ofRotate(270,0,0,1);
            ofSetColor(0,0,0,125);
            ofEnableAlphaBlending();
            ofDrawRectangle(infoPosition[i][0], infoPosition[i][1], 300, 70);
            ofDisableAlphaBlending();
            string status;
            stringstream ss;
            ss << "Booth Number: " << i+1 << endl
            << "Screen Tester: " << infoName[i] << endl
            << "Status: " << screenTestStatus[i] << endl;
            ofSetColor(255,255,255);
            ofDrawBitmapString(ss.str(), infoPosition[i][0]+20, infoPosition[i][1]+25);
            screenTestGrid.draw(infoPosition[i][0]+5, infoPosition[i][1]+screenTestGridDescent[i]+5,
                                screenTestGridSize[0]-10, screenTestGridSize[1]-10);
            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit()
{
}

//--------------------------------------------------------------
void ofApp::buttonPressed(int button)
{
    if(!isCountdown[button])
    {
        isCountdown[button] = true;
        pressButtonTime[button] = ofGetSystemTimeMillis();
        cdScreenShotNumber[button] = 0;
        if (mode[button] == "photo_booth")
        {
            cdEndTime[button] = ofGetElapsedTimeMillis() + cdTimeLength;
            screenShotQty[button] = photoBoothFps*5;
            cdNextScreenShotTime[button] = ofGetElapsedTimeMillis() + 1000;
            // button blinks only in photo booth mode
            for (int k = 0; k < serialDeviceQty; k++)
            {
                serialDevice[k].writeBytes(to_string(serialDeviceAssignments[button]) + "_blink");
            }
        }
        else if (mode[button] == "screen_test")
        {
            cdEndTime[button] = ofGetElapsedTimeMillis() + 3*60*1000;
            screenShotQty[button] = screenTestFps*180;  // 3m = 180s
            cdNextScreenShotTime[button] = ofGetElapsedTimeMillis();
            cout << "press!" << endl;
        }
    }
}

//--------------------------------------------------------------
string ofApp::generateScreenShotName(int time, int number, bool isPhotoBooth)
{
    string output;
    string s = "";
    if (isPhotoBooth)
    {
        output = to_string(time);
        // computer clock time
        int r0 = 10 - output.length();
        for (int i = 0; i < r0; i++)
        {
            output = "0" + output;
        }
        s = "_";
    }
    // image sequence number
    int r1 = 4 - to_string(number).length();
    for (int i = 0; i < r1; i++)
    {
        s = s + "0";
    }
    output = output + s + to_string(number);
    // performance session number
    // output = to_string(gui->performance) + "_" + output + ".jpg";
    output = output + ".jpg";
    return output;
}
