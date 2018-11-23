#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( )
{
    ofGLFWWindowSettings settings;
    settings.setSize(960, 540);
    settings.resizable = false;
    ofCreateWindow(settings);
    
	ofRunApp(new ofApp());
}
