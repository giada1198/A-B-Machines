#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    openFace.setup();
    // openFace.model_location = "/Users/giada/openFrameworks/apps/myApps/example/bin/data";
    grabber.initGrabber(OPENFACE_WIDTH,OPENFACE_HEIGHT);
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    if (grabber.isFrameNew()){
        openFace.setImage(grabber.getPixels());
    }

// Obtaining landmarks & other results
//     cv::Vec6d pose        = openFace.face.pose;
//     cv::Point3f gaze0     = openFace.face.gaze0;
//     cv::Point3f gaze1     = openFace.face.gaze1;
//     float certainty       = openFace.face.certainty;
//     cv::Rect_<float> rect = openFace.face.rect;
//     for (int ind = 0; ind < OPENFACE_N_KEYPOINTS; ind ++){
//         ofVec2f pt = openFace.face.getLandmark(ind);
//     }

    mask.begin();
    ofClear(0,0,0);
    ofSetColor(255);
    ofBeginShape();
    for(int i=48; i<60; i++) {
        ofVertex(openFace.face.getLandmark(i));
    }
    ofEndShape();
    mask.end();

    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofSetColor(255);
    grabber.draw(0,0);
    ofDisableBlendMode();
    
    if(isFaceDraw) {
        openFace.face.draw();
    }

    ofSetColor(255);
    // ofxCv::drawMat(openFace.canvas,640,0,320,240);
    ofDrawBitmapStringHighlight("App FPS: "+ofToString(ofGetFrameRate()),0,20,ofColor(0),ofColor(255,255,0));
    ofDrawBitmapStringHighlight("Tracker FPS: "+ofToString(openFace.fps),0,40,ofColor(0),ofColor(255,255,0));
    ofDrawBitmapStringHighlight("Certainty: "+ofToString(openFace.face.certainty),0,60,ofColor(0),ofColor(255,255,0));

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    isFaceDraw = !isFaceDraw;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}