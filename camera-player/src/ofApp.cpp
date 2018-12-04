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
    fboLips.allocate(660, 374, GL_RGB);
    
    mClient.setup();
    mClient.setApplicationName("Black Syphon");
    mClient.setServerName("");
    
    // oft.lock();
    // oft.unlock();
    // oft.startThread(true); // start the OpenFace thread
}

//--------------------------------------------------------------
void ofApp::update()
{
    while (oscReceiver.hasWaitingMessages())  // check for waiting messages
    {
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);
        if (m.getAddress() == "/camera")
        {
            string s = m.getArgAsString(0);
            if (s == "camera" && mode != "camera")
            {
                mode = "camera";
                ofSetFrameRate(24);
            }
//            else if (s == "lipsync" && mode != "lip_sync")
//            {
//                 oft.lock();
//                 oft.openFace.reset();
//                 oft.unlock();
//                 mode = "lip_sync";
//                 ofSetFrameRate(12);
//            }
            else if (s == "audience" && mode != "audience")
            {
                mode = "audience";
                ofSetFrameRate(6);
                loadAudienceImages();
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit()
{
    // oft.stopThread();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);
    if (mode == "audience" && hasLoadedAudience)
    {
        if (hasLoadedAudience)
        {
            ofPushMatrix();
            ofTranslate(960,0,0);
            ofRotate(90,0,0,1);
            for (int n=0; n<3; n++) {
                for (int m=0; m<3; m++)
                {
                    int x = 270*(int(((n+1)*3+m)/6));
                    int y = 160*(((n+1)*3+m)%6);
                    // int screen;
                    // if (n == 3) { screen = 1; }
                    // else { screen = n; }
                    ofImage img;
                    // string path = "../../../data/copy/" + imgAudienceDirName[screen] + "/" + dirAudience[screen][currentImageAudience[n][m]];
                    string path = "../../../data/copy/" + imgAudienceDirName[n] + "/" + dirAudience[n][currentImageAudience[n][m]];
                    img.load(path);
                    img.draw(x,y,270,160);
                    if (n == 1) { img.draw(0,160*m,270,160); }
                    currentImageAudience[n][m] += 2;
                    // currentImageAudience[n][m] %= dirAudience[screen].size();
                    currentImageAudience[n][m] %= dirAudience[n].size();
                }
            }
            ofPopMatrix();
        }
    }
//    else if (mode == "lip_sync")
//    {
//        if (isOpenFaceFirstRun)
//        {
//            mode = "camera";
//            isOpenFaceFirstRun = false;
//        }
//        fbo1.begin();
//        mClient.draw(-20,-30);
//        fbo1.end();
//        fbo2.begin();
//        mClient.draw(-365,-30);
//        fbo2.end();
//        fbo3.begin();
//        mClient.draw(-20,-271);
//        fbo3.end();
//
//        fboLips.begin();
//        fbo1.draw(0,0,330,187);
//        fbo2.draw(330,0,330,187);
//        fbo3.draw(330,187,330,187);
//        fboLips.end();
//
//        oft.lock();
//        ofPixels pixs;
//        fboLips.readToPixels(pixs);
//        oft.pixels = pixs;
//
//        for (int i = 0; i < oft.openFace.faces.size(); i++)
//        {
//            if (oft.openFace.faces[i].active)
//            {
//                int x0 = (oft.openFace.faces[i].getLandmark(48))[0];
//                int y0 = (oft.openFace.faces[i].getLandmark(48))[1];
//                int x1 = y0;
//                int y1 = y0;
//                for (int j=49; j<60; j++)
//                {
//                    int xn = (oft.openFace.faces[i].getLandmark(j))[0];
//                    int yn = (oft.openFace.faces[i].getLandmark(j))[1];
//                    if (xn < x0) { x0 = xn; }
//                    if (yn < y0) { y0 = yn; }
//                    if (xn > x1) { x1 = xn; }
//                    if (yn > y1) { y1 = yn; }
//                }
//                int w = abs(x0-x1);
//                int h = abs(y0-y1);
//                if (w > 0)
//                {
//                    ofFbo lip;
//                    lip.allocate(w, h, GL_RGB);
//                    lip.begin();
//                    fboLips.draw(-x0, -y0, 660, 374);
//                    lip.end();
//                    int cx = x0+w/2;
//                    int cy = y0+h/2;
//                    int x2, y2;
//                    if (cx < 330 && cy < 187)
//                    {
//                        x2 = 240-w*2;
//                        y2 = 135-h*2;
//                        lip.draw(x2, y2, w*4, h*4);
//                    }
//                    else if (cx > 330 && cy < 187)
//                    {
//                        x2 = 480+240-w*2;
//                        y2 = 135-h*2;
//                        lip.draw(x2, y2, w*4, h*4);
//                    }
//                    else
//                    {
//                        x2 = 240-w*2;
//                        y2 = 270+135-h*2;
//                        lip.draw(x2, y2, w*4, h*4);
//                    }
//                }
//            }
//        }
//        oft.unlock();
//    }
    else if (mode == "camera")
    {
        ofSetColor(255);
        fbo1.begin();
        mClient.draw(-20,-30);
        fbo1.end();
        fbo2.begin();
        mClient.draw(-365,-30);
        fbo2.end();
        fbo3.begin();
        mClient.draw(-20,-271);
        fbo3.end();
        // draw cameras on canvas
        fbo1.draw(0,0,480,270);
        fbo2.draw(480,0,480,270);
        fbo3.draw(0,270,480,270);
    }
    syphonServer.publishScreen();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == '1' && mode != "camera")
    {
        mode = "camera";
        ofSetFrameRate(24);
    }
//    else if (key == '2' && mode != "lip_sync")
//    {
//        oft.lock();
//        oft.openFace.reset();
//        oft.unlock();
//        mode = "lip_sync";
//        ofSetFrameRate(12);
//    }
    else if (key == '3' && mode != "audience")
    {
        mode = "audience";
        ofSetFrameRate(6);
        loadAudienceImages();
    }
}

//--------------------------------------------------------------
bool ofApp::loadImagesDir(vector<string>& dir, string name)
{
    dir.clear();
    string datafolder = "../../../data/copy/" + name + "/";
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
        for (int screen=0; screen<4; screen++)
        {
            int b;
            if (screen == 3) { b = 1; }
            else { b = screen; }
            for (int i=0; i<3; i++)
            {
                currentImageAudience[screen][i] = (30*b+(int)ofRandom(-1,1000))%dirAudience[b].size();
            }
        }
    }
}

//--------------------------------------------------------------
inline string ofApp::exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd,"r"),pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr){
            ofLogVerbose()<<"reading exec results...";
            result += buffer.data();
        }
    }
    return result;
}

//--------------------------------------------------------------
inline vector<string> ofApp::split(const string& str, const string& delim)
{
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
