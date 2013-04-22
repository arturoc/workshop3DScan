#pragma once

#include "ofMain.h"
#include "ofxGraycode.h"
#include "ofxV4L2Settings.h"
#include "ofxGui.h"

using namespace ofxGraycode;

class testApp : public ofBaseApp{

	public:
		void setup(ofWindowEventArgs & window);
		void update(ofWindowEventArgs & window);
		void draw(ofWindowEventArgs & window);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		Encoder encoder;
		Decoder decoder1, decoder2;
		PayloadGraycode payload;
		ofVideoGrabber grabber1, grabber2;
		int next1, next2;
		ofImage output;
		bool start,end,showDecoder;
		ofDirectory captures1,captures2;
		int frame;
		ofImage preview1, preview2;
		ofxV4L2Settings settings1, settings2;
		ofxPanel gui;
		int imageHeight;
};
