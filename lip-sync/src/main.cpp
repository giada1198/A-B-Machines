#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( )
{
    ofGLFWWindowSettings settings;
    settings.setSize(735, 600);
    settings.resizable = false;
    ofCreateWindow(settings);
    
    ofRunApp(new ofApp());
}
