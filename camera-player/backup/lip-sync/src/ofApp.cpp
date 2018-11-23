#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("A/B Machines Media Player");
    ofSetFrameRate(12); // run at 60 fps
    ofSetVerticalSync(true);
    
    // listen on the given port
    ofLog() << "listening for osc messages on port " << PORT;
    oscReceiver.setup(PORT);
    syphonServer.setName("Output");
    
    // load images from folders
    loadImages(dirJackie, imgJackie, "Jackie");
    loadImages(dirLiz, imgLiz, "Liz");
    loadImages(dirMarilyn, imgMarilyn, "Marilyn");
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
        if (m.getAddress() == "/player")
        {
            string s = m.getArgAsString(0);
            if (s == "jackie")
            {
                mode = "jackie";
                currentImage = 0;
            }
            else if (s == "liz")
            {
                mode = "liz";
                currentImage = 0;
            }
            else if (s == "marilyn")
            {
                mode = "marilyn";
                currentImage = 0;
            }
            else if (s == "none")
            {
                mode = "none";
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);
    if (mode != "none")
    {
        ofPushMatrix();
        ofTranslate(735,0,0);
        ofRotate(90,0,0,1);
        if (mode == "jackie" && dirJackie.size()>0)
        {
            imgJackie[currentImage].draw(0,0,600,735);
            currentImage++;
            currentImage %= dirJackie.size();
        }
        else if (mode == "liz" && dirLiz.size()>0)
        {
            imgLiz[currentImage].draw(0,0,600,735);
            currentImage++;
            currentImage %= dirLiz.size();
            cout << "lol" << endl;
        }
        else if (mode == "marilyn" && dirMarilyn.size()>0)
        {
            imgMarilyn[currentImage].draw(0,0,600,735);
            currentImage++;
            currentImage %= dirMarilyn.size();
        }
        ofPopMatrix();
    }
    syphonServer.publishScreen();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == '1')
    {
        mode = "jackie";
        currentImage = 0;
    }
    else if (key == '2')
    {
        mode = "liz";
        currentImage = 0;
    }
    else if (key == '3')
    {
        mode = "marilyn";
        currentImage = 0;
    }
    else if (key == '0')
    {
        mode = "none";
    }
}

//--------------------------------------------------------------
void ofApp::loadImages(ofDirectory& dir, vector<ofImage>& img, string name)
{
    dir.listDir(name + "/");
    dir.allowExt("jpg");
    dir.sort();
    // allocate the vector to have as many ofImages as files
    if (dir.size())
    {
        img.assign(dir.size(), ofImage());
    }
    // load images into the ofImage vector
    for (int i=0; i<(int)dir.size(); i++)
    {
        if (name == "Jackie")
        {
            threadedImageloader1.loadFromDisk(img[i], dir.getPath(i));
        }
        else if (name == "Liz")
        {
            threadedImageloader2.loadFromDisk(img[i], dir.getPath(i));
        }
        else if (name == "Marilyn")
        {
            threadedImageloader3.loadFromDisk(img[i], dir.getPath(i));
        }
    }
}
