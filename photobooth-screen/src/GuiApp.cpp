#include "GuiApp.h"

//--------------------------------------------------------------
void GuiApp::setup(){
	parameters.setName("parameters");
    parameters.add(isDebug.set("debug", false));
    
	gui.setup(parameters);
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
