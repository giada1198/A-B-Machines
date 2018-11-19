#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowTitle("A/B Machines Camera Syphoner");
    ofSetFrameRate(12);
    ofSetVerticalSync(false);
    
    // listen on the given port
    ofLog() << "listening for osc messages on port " << PORT;
    oscReceiver.setup(PORT);
    syphonServer.setName("Output");
    
    fbo1.allocate(330, 187, GL_RGB);
    fbo2.allocate(330, 187, GL_RGB);
    fbo3.allocate(330, 187, GL_RGB);
    fbo4.allocate(960, 540, GL_RGB);
    
    mClient.setup();
    mClient.setApplicationName("Black Syphon");
    mClient.setServerName("");
    
    oft.lock();
    oft.unlock();
    oft.startThread(true); // start the OpenFace thread
}

//--------------------------------------------------------------
void ofApp::update()
{
    // check for waiting messages
    while (oscReceiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);
        if (m.getAddress() == "/lip")
        {
            string s = m.getArgAsString(0);
            if (s == "on" && !isLipSync)
            {
                oft.lock();
                oft.openFace.reset();
                oft.unlock();
                ofSetFrameRate(12);
                isLipSync = true;
            }
            else if (s == "off" && isLipSync)
            {
                ofSetFrameRate(24);
                isLipSync = false;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit()
{
    oft.stopThread();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);

    fbo1.begin();
    mClient.draw(-20,-30);
    fbo1.end();
    fbo2.begin();
    mClient.draw(-365,-30);
    fbo2.end();
    fbo3.begin();
    mClient.draw(-20,-271);
    fbo3.end();
    
    if (isLipSync)
    {
        fbo4.begin();
        ofClear(0,0,0);
        fbo1.draw(0,0,480,270);
        fbo2.draw(480,0,480,270);
        fbo3.draw(0,270,480,270);
        fbo4.end();
        
        // fbo4.draw(0,0);
        
        oft.lock();
        ofPixels pixs;
        fbo4.readToPixels(pixs);
        oft.pixels = pixs;

        mask.begin();
        ofClear(0,0,0);
        ofSetColor(255);

        int cnt = 0; // face count
        for (int i = 0; i < oft.openFace.faces.size(); i++)
        {
            if (oft.openFace.faces[i].active){ // do something only when the face model is active
                cnt += 1;
                ofBeginShape();
                for (int j=48; j<60; j++)
                {
                    ofVertex(oft.openFace.faces[i].getLandmark(j));
                }
                ofEndShape();
                
                // send the faces to trackers
                tkr.update(oft.openFace.faces);
                
                // query the face tracker for face labels
                for (int i = 0; i < tkr.tracked_faces.size(); i++)
                {
                    ofDrawBitmapStringHighlight("#"+ofToString(tkr.tracked_faces[i].label),
                                                tkr.tracked_faces[i].currPos.x,
                                                tkr.tracked_faces[i].currPos.y,
                                                ofColor(0),ofColor(255,255,0));
                }
            }
        }
        mask.end();
        oft.unlock();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        fbo4.draw(0,0);
        ofDisableBlendMode();
    }
    else
    {
        fbo1.draw(0,0,480,270);
        fbo2.draw(480,0,480,270);
        fbo3.draw(0,270,480,270);
    }
    syphonServer.publishScreen();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == 'r')
    {
        isLipSync = !isLipSync;
//        oft.lock();
//        oft.openFace.reset();
//        oft.unlock();
    }
}
