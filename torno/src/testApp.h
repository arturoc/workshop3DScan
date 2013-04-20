#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxGui.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void selectionSideChanged(int & side);

		ofSerial serial;
		unsigned char buffer[1024];
		ofxKinect kinect;
		ofVboMesh mesh;
		ofEasyCam camFront;
		ofCamera camTop;

		ofParameter<float> nearClip, farClip;
		ofParameter<ofVec3f> camtopPos;
		ofParameter<ofVec3f> topLookAt, topUp;
		ofParameter<int> selectionSide;
		ofParameter<float> angle;
		ofxPanel gui;
		ofVboMesh addMesh;
		ofVboMesh selectionQuad;
		ofVboMesh center;

		int steps;
		int stepsOneTurn;
		int numTurns;
		ofMatrix4x4 rotationMat;
};
