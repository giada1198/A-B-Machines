#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowTitle("A/B Machines Camera Syphoner");
    ofSetFrameRate(24);
    ofSetVerticalSync(false);
    
    // listen on the given port
    ofLog() << "listening for osc messages on port " << PORT;
    oscReceiver.setup(PORT);
    syphonServer.setName("Output");
    
    fbo.allocate(720, 486, GL_RGB);
    fbo1.allocate(330, 187, GL_RGB);
    fbo2.allocate(330, 187, GL_RGB);
    fbo3.allocate(330, 187, GL_RGB);
    
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
    // grabber.update();
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
    
    fbo1.draw(0,0,480,270);
    fbo2.draw(480,0,480,270);
    fbo3.draw(0,270,480,270);
    
    syphonServer.publishScreen();

//    fbo.begin();
//    mClient.draw(0,0);
//    fbo.end();
//    fbo.draw(0,0);

//    ofBackground(0);
//    oft.lock();  // lock the thread for reading
//    fbo.begin();
//    // ofSetColor(0);
//
//    mClient.draw(0,0);
//
//    fbo.end();
//    ofPixels pixels;
//    fbo.readToPixels(pixels);
//    oft.pixels = pixels; // send the camera pixels to openFace
//    // if (bRunNewFrameOnly) { oft.bFrameNew = grabber.isFrameNew(); } // run on new frames only?
//
//    //
//    fbo.draw(0,0);
//
//    int cnt = 0; // face count
//    for (int i = 0; i < oft.openFace.faces.size(); i++)
//    {
//        if (oft.openFace.faces[i].active){ // do something only when the face model is active
//            cnt += 1;
//            ofColor col = ofColor(255,i*255/(float)oft.openFace.num_faces_max,0);
//            oft.openFace.faces[i].draw(col); // draw debug
//        }
//    }
//    // send the faces to trackers
//    tkr.update(oft.openFace.faces);
//    // query the face tracker for face labels
//    for (int i = 0; i < tkr.tracked_faces.size(); i++)
//    {
//        ofDrawBitmapString("#"+ofToString(tkr.tracked_faces[i].label),
//                           tkr.tracked_faces[i].currPos.x,
//                           tkr.tracked_faces[i].currPos.y-5);
//    }
//    oft.unlock(); // release the lock for OpenFace
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
//    if (key == 'r')
//    {
//        oft.lock();
//        oft.openFace.reset();
//        oft.unlock();
//    }
}
