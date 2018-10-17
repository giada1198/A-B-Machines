#include "GuiApp.h"

//--------------------------------------------------------------
void GuiApp::setup(){
	parameters.setName("parameters");
//    parameters.add(radius.set("radius",50,1,100));
//    parameters.add(color.set("color",100,ofColor(0,0),255));
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
