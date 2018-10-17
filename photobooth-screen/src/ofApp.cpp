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
    serialSetup();
    
    vidGrabber[0].listDevices();
    for (int i = 0; i < cameraQty ; i++)
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
}

//--------------------------------------------------------------

void ofApp::serialSetup()
{
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    if (!devicesInfo.empty())
    {
        deviceQty = 1;
    
        ofLogNotice("ofxSerial") << "Connected Devices: ";
        for(int i = 0; i < deviceQty; i++)
        {
            ofLogNotice("ofxSerial") << devicesInfo[i];
            bool success = device[i].setup(devicesInfo[i], 9600);
            if(success)
            {
                ofLogNotice("ofxSerial") << "Successfully setup " << devicesInfo[deviceAssignments[i]];
            }
            else
            {
                ofLogNotice("ofxSerial") << "Unable to setup " << devicesInfo[deviceAssignments[i]];
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
    for (int i = 0; i < cameraQty ; i++)
    {
        vidGrabber[i].update();
        fbo[i].begin();
        vidGrabber[i].draw( fboPositions[i][0], fboPositions[i][1],
                           -fboSizes[i][0],     fboSizes[i][1]);
        fbo[i].end();
    }
    // Serial
    try
    {
        unsigned char buffer[1024]; // Read all bytes from the device;
        for(int i = 0; i < deviceQty; i++)
        {
            while(device[i].available() > 0)
            {
                string str;
                int sz = device[i].readBytes(buffer, 1024);
                for(int i = 0; i < sz ; ++i)
                {
                    str += buffer[i];
                }
                ofLogNotice("ofxSerial") << str;
                for(int k = 0; k < cameraQty; k++)
                {
                    if(str == to_string(k) + "_press")
                    {
                        buttonPressed(k);
                        cout << "hi" << endl;
                    }
                }
            }
        }
                // Send some new bytes to the device to have them echo'd back.
        //        std::string text = "Frame Number: " + ofToString(ofGetFrameNum());
        //
        //        ofx::IO::ByteBuffer textBuffer(text);
        //
        //        device.writeBytes(textBuffer);
        //        device.writeByte('\n');
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

    for (int i = 0; i < cameraQty ; i++) {
        
        fbo[i].draw(cameraPositions[i][0], cameraPositions[i][1]);
        if(hasShot[i] && isCountdown[i])
        {
            screenshot[i].draw(cameraPositions[i][0], cameraPositions[i][1]);
        }
        
        // Draw Countdown Graphic
        if(isCountdown[i])
        {
            if(hasShot[i])
            {
                screenshot[i].draw(cameraPositions[i][0], cameraPositions[i][1]);
            }
            
            ofEnableAlphaBlending();
            float x = cdEndTime[i] - ofGetElapsedTimeMillis();
            
            if(x >= cdTimeLength-1000)
            {
                ofSetColor(0,0,0,125);
                ofDrawRectangle(cameraPositions[i][0], cameraPositions[i][1], 260, 75);
                ofSetColor(255,255,255);
                ofDrawBitmapString("Get Ready!", cameraPositions[i][0]+20, cameraPositions[i][1]+40);
            }
            else if(x > 1000)
            {
                ofSetColor(0,0,0,125);
                ofDrawRectangle(cameraPositions[i][0], cameraPositions[i][1], 260, 75);
                ofSetColor(255,255,255);
                ofDrawBitmapString(to_string(int(x/1000)), cameraPositions[i][0]+20, cameraPositions[i][1]+40);
            }
            else if(x >= 0)
            {
                if(!hasShot[i])
                {
                    screenshot[i].grabScreen(cameraPositions[i][0],
                                             cameraPositions[i][1],
                                             cameraPositions[i][0] + cameraSizes[0],
                                             cameraPositions[i][1] + cameraSizes[1]);
                    screenshot[i].save("screenshot.jpg");
                    hasShot[i] = true;
                }
                ofSetColor(0,0,0,125);
                ofDrawRectangle(cameraPositions[i][0], cameraPositions[i][1], 260, 75);
                ofSetColor(255,255,255);
                ofDrawBitmapString("Shoot!", cameraPositions[i][0]+20, cameraPositions[i][1]+40);
                ofSetColor(255,255,255,(int(255*(1-abs(500-x)/500))));
                ofDrawRectangle(cameraPositions[i][0], cameraPositions[i][1],
                                cameraPositions[i][0] + cameraSizes[0],
                                cameraPositions[i][1] + cameraSizes[1]);
            }
            else if(x <= -cdFreezeLength)
            {
                isCountdown[i] = false;
                hasShot[i] = false;
            }
            ofDisableAlphaBlending();
        }
    }

//    stringstream ss;
//    ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
//    << "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
//    << "FPS: " << ofGetFrameRate() << endl
//    << (bRecording?"pause":"start") << " recording: r" << endl
//    << (bRecording?"close current video file: c":"") << endl;
//    ofSetColor(0,0,0,100);
//    ofDrawRectangle(0, 0, 260, 75);
//    ofSetColor(255, 255, 255);
//    ofDrawBitmapString(ss.str(),15,15);
    
//    ofSetColor(gui->color);
//    ofDrawCircle(ofGetWidth()*0.5,ofGetHeight()*0.5,gui->radius);
	ofSetColor(255,255,255);
	ofDrawBitmapString(ofGetFrameRate(),20,20);
}

//--------------------------------------------------------------
void ofApp::exit()
{
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    for (int i = 0; i < deviceQty ; i++) {
        switch(key)
        {
            case 'q':
                buttonPressed(0);
                break;
            case 'w':
                buttonPressed(1);
                break;
            case 'e':
                buttonPressed(2);
                break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::buttonPressed(int button)
{
    if(!isCountdown[button])
    {
        isCountdown[button] = true;
        cdEndTime[button] = ofGetElapsedTimeMillis() + cdTimeLength;
        for(int k = 0; k < deviceQty; k++)
        {
            device[k].writeBytes(to_string(button) + "_blink");
        }
    }
}

//--------------------------------------------------------------

void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
