#pragma once

#include "ofMain.h"
#include "ofxKinect.h"

class testApp : public ofBaseApp
{

public:

    // application lifecycle methods
    void setup();
    void update();
    void draw();

    // UI methods
    void keyReleased(int key);
    void mouseReleased(int x, int y, int button);
    void keyPressed(int key);

    ofxKinect kinect;
    ofMesh mesh;
    ofEasyCam camera;

    int zThreshold = 1500;

    bool bRec = false;


private:
    // helpers

};

