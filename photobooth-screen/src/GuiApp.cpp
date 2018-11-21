#include "GuiApp.h"

//--------------------------------------------------------------
void GuiApp::setup()
{
    button[0].addListener(this, &GuiApp::button1Pressed);
    button[1].addListener(this, &GuiApp::button2Pressed);
    button[2].addListener(this, &GuiApp::button3Pressed);

	parameters.setName("A/B Machines");
    // parameters.add(performance.set("Performance:",1,1,8));
    // parameters.add(performanceLabel.set(sessions[0]));
    // parameters.add(isScreenTest.set("Screen Test", false));
    for (int i = 0; i < 3; i++)
    {
        parameters.add(isScreenTest[i].set("Screen Test Booth " + to_string(i+1), false));
    }
    gui.setup(parameters);
    gui.add(button[0].setup("Button 1 Jackie"));
    gui.add(button[1].setup("Button 2 Liz"));
    gui.add(button[2].setup("Button 3 Marilyn"));

    // listen on the given port
    ofLog() << "listening for osc messages on port " << PORT;
    oscReceiver.setup(PORT);

	ofBackground(0);
	ofSetVerticalSync(false);
}

//--------------------------------------------------------------
void GuiApp::update()
{
    // check for waiting messages
    while (oscReceiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);
        if (m.getAddress() == "/photobooth")
        {
            string s = m.getArgAsString(0);
            for (int i = 0; i > 3; i++)
            {
                if (s == (to_string(i) + "_photobooth") && isScreenTest[i]) { isScreenTest[i] = false; }
                else if (s == (to_string(i) + "_screentest") && !isScreenTest[i]) { isScreenTest[i] = true; }
            }
        }
    }
}

//--------------------------------------------------------------
void GuiApp::draw()
{
	gui.draw();
}

//--------------------------------------------------------------
// void GuiApp::updateLabel(int& val)
// {
//     performanceLabel.set(sessions[val-1]);
// }

//--------------------------------------------------------------
void GuiApp::button1Pressed() { isVirtualButtonPressed[0].set(true); }
void GuiApp::button2Pressed() { isVirtualButtonPressed[1].set(true); }
void GuiApp::button3Pressed() { isVirtualButtonPressed[2].set(true); }
