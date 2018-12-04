#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("A/B Machines Media Player");
    ofSetFrameRate(6); // run at 12 fps
    ofSetVerticalSync(true);

    // listen on the given port
    ofLog() << "listening for osc messages on port " << PORT;
    oscReceiver.setup(PORT);
    syphonServer.setName("Output");

    for (int k=0; k<3; k++) { syphonAudience[k].setName("Audience " + to_string(k+1)); }
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
            if (s == "jackie" && loadImagesDir(dirJackie, "Jackie"))
            {
                mode = "jackie";
                ofSetFrameRate(6);
                currentImage = 0;
            }
            else if (s == "liz" && loadImagesDir(dirLiz, "Liz"))
            {
                mode = "liz";
                ofSetFrameRate(6);
                currentImage = 0;
            }
            else if (s == "marilyn" && loadImagesDir(dirMarilyn, "Marilyn"))
            {
                mode = "marilyn";
                ofSetFrameRate(6);
                currentImage = 0;
            }
            else if (s == "audience" && hasLoadedAudience)
            {
                ofSetFrameRate(12);
                playAudienceImages();
            }
            else if (s == "load")
            {
                loadAudienceImages();
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
        if (mode == "audience") {
            for (int n=0; n<3; n++) {
                ofBackground(0);
                for (int m=0; m<15; m++) {
                    int x = 200*(int(m/5));
                    int y = 147.5*(m%5);
                    imgAudience[n][currentImageAudience[n][m]].draw(x,y,200,147.5);
                    currentImageAudience[n][m]++;
                    currentImageAudience[n][m] %= dirAudience[n].size();
                }
                syphonAudience[n].publishScreen();
            }
        }
        else if (mode == "jackie")
        {
            ofImage img;
            img.load("../../../data/copy/Jackie/" + dirJackie[currentImage]);
            img.draw(0,0,600,735);
            currentImage++;
            currentImage %= dirJackie.size();
        }
        else if (mode == "liz")
        {
            ofImage img;
            img.load("../../../data/copy/Liz/" + dirLiz[currentImage]);
            img.draw(0,0,600,735);
            currentImage++;
            currentImage %= dirLiz.size();
        }
        else if (mode == "marilyn" && dirMarilyn.size()>0)
        {
            ofImage img;
            img.load("../../../data/Marilyn/" + dirMarilyn[currentImage]);
            img.draw(0,0,600,735);
            currentImage++;
            currentImage %= dirMarilyn.size();
        }
        ofPopMatrix();
    }
    if (mode != "audience")
    {
        syphonServer.publishScreen();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == '1' && loadImagesDir(dirJackie, "Jackie"))
    {
        mode = "jackie";
        ofSetFrameRate(6);
        currentImage = 0;
    }
    else if (key == '2' && loadImagesDir(dirLiz, "Liz"))
    {
        mode = "liz";
        ofSetFrameRate(6);
        currentImage = 0;
    }
    else if (key == '3' && loadImagesDir(dirMarilyn, "Marilyn"))
    {
        mode = "marilyn";
        ofSetFrameRate(6);
        currentImage = 0;
    }
    else if (key == '4' && hasLoadedAudience)
    {
        ofSetFrameRate(12);
        playAudienceImages();
    }
    else if (key == 'l') { loadAudienceImages(); }
    else if (key == '0') { mode = "none"; }
}

//--------------------------------------------------------------
bool ofApp::loadImagesDir(vector<string>& dir, string name)
{
    dir.clear();
    string datafolder;
    if (name == "Marilyn")
    {
        datafolder = "../../../data/" + name + "/";
    }
    else
    {
        datafolder = "../../../data/copy/" + name + "/";
    }
    dir = split( exec(("cd " + datafolder + "; ls").c_str()), "\n" );
    return dir.size()>0;
}

//--------------------------------------------------------------
void ofApp::loadAudienceImages()
{
    bool loadSucceed = loadImagesDir(dirAudience[0], imgAudienceDirName[0]) &&
    loadImagesDir(dirAudience[1], imgAudienceDirName[1]) &&
    loadImagesDir(dirAudience[2], imgAudienceDirName[2]);
    if (loadSucceed)
    {
        hasLoadedAudience = true;
        for (int n=0; n<3; n++) {
            imgAudience[n].clear();
            imgAudience[n].assign(dirAudience[n].size(), ofImage());
            for (int m=0; m<dirAudience[n].size(); m++) {
                string path = "../../../data/copy/" + imgAudienceDirName[n] + "/" + dirAudience[n][m];
                threadedImageloader[n].loadFromDisk(imgAudience[n][m], path);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::playAudienceImages()
{
    mode = "audience";
    for (int n=0; n<3; n++)
    {
        for (int m=0; m<15; m++)
        {
            currentImageAudience[n][m] = (30*m+(int)ofRandom(-1,1000))%dirAudience[n].size();
        }
    }
}

//--------------------------------------------------------------
inline string ofApp::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd,"r"),pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())){
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr){
            ofLogVerbose()<<"reading exec results...";
            result += buffer.data();
        }
    }
    return result;
}

//--------------------------------------------------------------
inline vector<string> ofApp::split(const string& str, const string& delim) {
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

