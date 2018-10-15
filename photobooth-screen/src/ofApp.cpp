#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255);
	ofSetCircleResolution(200);
    if(!singleCamera)
    {
        screenSetup.setup(ofGetWidth(), ofGetHeight(), ofxScreenSetup::MONITOR_2);
    }
    
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_VERBOSE);

//    fileName = "testMovie";
//    fileExt = ".mov"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats
//    vidRecorder.setVideoCodec("mpeg4");
//    vidRecorder.setVideoBitrate("800k");
//    vidRecorder.setAudioCodec("mp3");
//    vidRecorder.setAudioBitrate("192k");

//    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    bRecording = false;
    ofEnableAlphaBlending();

    if(singleCamera) cameraAmount = 1;
    else cameraAmount = 3;

    vidGrabber[0].listDevices();
    int cameraSelction[3] = { 2, 1, 0 };
    
    for (int i = 0; i < cameraAmount ; i++)
    {
        vidGrabber[i].setDeviceID(cameraSelction[i]);
        vidGrabber[i].initGrabber(960,544);
    }
    
    // Connect to Press Buttons
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    ofLogNotice("ofxSerial") << "Connected Devices: ";
    for(int i = 0; i < devicesInfo.size(); i++)
    {
        ofLogNotice("ofxSerial") << devicesInfo[i];
    }
    if (!devicesInfo.empty())
    {
        for(int i = 0; i < deviceQty; i++)
        {
            bool success = device[i].setup(devicesInfo[deviceNumber[i]], 9600);
            if(success)
            {
                ofLogNotice("ofxSerial") << "Successfully setup " << devicesInfo[deviceNumber[i]];
            }
            else
            {
                ofLogNotice("ofxSerial") << "Unable to setup " << devicesInfo[deviceNumber[i]];
            }
        }
    }
    else
    {
        ofLogNotice("ofxSerial") << "No devices connected.";
    }
}

//--------------------------------------------------------------
void ofApp::update(){

    for (int i = 0; i < cameraAmount ; i++)
    {
        vidGrabber[i].update();
    }

    if(vidGrabber[0].isFrameNew() && bRecording){
        bool success = vidRecorder.addFrame(vidGrabber[0].getPixels());
        if (!success) {
            ofLogWarning("This frame was not added!");
        }
    }
    
    try
    {
        unsigned char buffer[1024]; // Read all bytes from the device;
        for(int i = 0; i < deviceQty; i++)
        {
            while (device[i].available() > 0)
            {
                string str;
                int sz = device[i].readBytes(buffer, 1024);
                for (int i = 0; i < 7 ; ++i)
                {
                    str += buffer[i];
                }
                ofLogNotice("ofxSerial") << str;
                if(str == to_string(i) + "_press")
                {
                    device[i].writeBytes(to_string(i) + "_blink\n");
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

//    if (vidRecorder.hasVideoError()) {
//        ofLogWarning("The video recorder failed to write some frames!");
//    }
//    if (vidRecorder.hasAudioError()) {
//        ofLogWarning("The video recorder failed to write some audio samples!");
//    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    


    ofBackground(0,0,0);
    ofSetColor(255,255,255);

    for (int i = 0; i < cameraAmount ; i++) {
        vidGrabber[i].draw(cameraPositions[i][0], cameraPositions[i][1]);
    }

//    stringstream ss;
//    ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
//    << "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
//    << "FPS: " << ofGetFrameRate() << endl
//    << (bRecording?"pause":"start") << " recording: r" << endl
//    << (bRecording?"close current video file: c":"") << endl;
    
    ofSetColor(0,0,0,100);
    ofDrawRectangle(0, 0, 260, 75);
    ofSetColor(255, 255, 255);
//    ofDrawBitmapString(ss.str(),15,15);
    
    if(bRecording){
        ofSetColor(255, 0, 0);
        ofDrawCircle(ofGetWidth() - 20, 20, 5);
    }
    
    //
	ofSetColor(gui->color);
	ofDrawCircle(ofGetWidth()*0.5,ofGetHeight()*0.5,gui->radius);
	ofSetColor(0);
	ofDrawBitmapString(ofGetFrameRate(),20,20);
}

void ofApp::exit(){
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key=='r'){
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized()) {
            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, vidGrabber[0].getWidth(), vidGrabber[0].getHeight(), 30);
            // vidRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels, "-vcodec mpeg4 -b 1600k -acodec mp2 -ab 128k -f mpegts udp://localhost:1234"); // for custom ffmpeg output string (streaming, etc)
            vidRecorder.start();
        }
        else if(!bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(true);
        }
        else if(bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(false);
        }
    }
    if(key=='c'){
        bRecording = false;
        vidRecorder.close();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
