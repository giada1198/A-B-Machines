#include "GuiApp.h"

//--------------------------------------------------------------
void GuiApp::setup()
{
    button[0].addListener(this, &GuiApp::button1Pressed);
    button[1].addListener(this, &GuiApp::button2Pressed);
    button[2].addListener(this, &GuiApp::button3Pressed);
    
	parameters.setName("A/B Machines");
    parameters.add(performance.set("Performance:",1,1,8));
    parameters.add(performanceLabel.set(sessions[0]));
    parameters.add(isScreenTest.set("Screen Test", false));
    
    gui.setup(parameters);
    gui.add(button[0].setup("Button 1 Jackie"));
    gui.add(button[1].setup("Button 2 Liz"));
    gui.add(button[2].setup("Button 3 Marilyn"));
    
	ofBackground(0);
	ofSetVerticalSync(false);
}

//--------------------------------------------------------------
void GuiApp::update()
{
}

//--------------------------------------------------------------
void GuiApp::draw()
{
	gui.draw();
}

//--------------------------------------------------------------
void GuiApp::updateLabel(int& val)
{
    performanceLabel.set(sessions[val-1]);
}

//--------------------------------------------------------------
void GuiApp::button1Pressed() { isVirtualButtonPressed[0].set(true); }
void GuiApp::button2Pressed() { isVirtualButtonPressed[1].set(true); }
void GuiApp::button3Pressed() { isVirtualButtonPressed[2].set(true); }
