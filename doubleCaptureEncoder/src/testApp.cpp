#include "testApp.h"

int width = 1024;
int height = 768;
float scale = 1;

//--------------------------------------------------------------
void testApp::setup(ofWindowEventArgs & window){
	cout << "setup " << window.window->getID() << endl;
	ofSetVerticalSync(true);
	if(window.window->getID()!=0) return;
	ofSetFrameRate(10);
	payload.init(width/scale,height/scale);
	grabber1.setDeviceID(2);
	grabber1.initGrabber(1280,720);
	grabber2.setDeviceID(3);
	grabber2.initGrabber(1280,720);
	encoder.init(payload);
	decoder1.init(payload);
	decoder2.init(payload);
	next1 = next2 = 0;
	end = false;
	string captureStr = ofGetTimestampString();
	captures1.open(captureStr+"/capture1");
	captures1.create(true);
	captures2.open(captureStr+"/capture2");
	captures2.create(true);
	frame = -1;
	start = false;
	imageHeight = grabber1.getHeight()*640/grabber1.getWidth();

	settings1.setup("/dev/video1");
	settings2.setup("/dev/video2");
	gui.setup("","settings.xml",10,imageHeight+20);
	gui.add(settings1.parameters);
	gui.add(settings2.parameters);
	gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void testApp::update(ofWindowEventArgs & window){
	if(window.window->getID()!=0) return;

	grabber1.update();
	grabber2.update();

	if(!start || end) return;

	if(grabber1.isFrameNew()){
		next1++;
		if(frame>=0 && next1==3){
			ofSaveImage(grabber1.getPixelsRef(),ofFilePath::join(captures1.getAbsolutePath(),ofToString(frame)+".png"));
			decoder1 << grabber1.getPixelsRef();
		}
	}
	if(grabber2.isFrameNew()){
		next2++;
		if(frame>=0 && next2==3){
			ofSaveImage(grabber2.getPixelsRef(),ofFilePath::join(captures2.getAbsolutePath(),ofToString(frame)+".png"));
			decoder2 << grabber2.getPixelsRef();
		}
	}

	if(next1>=3 && next2>=3){
		if (!(encoder >> output)){
			ofLogNotice() << "End of frames";
			end = true;
			frame=0;
			preview1.loadImage(ofFilePath::join(captures1.getAbsolutePath(),ofToString(frame)+".png"));
			preview2.loadImage(ofFilePath::join(captures2.getAbsolutePath(),ofToString(frame)+".png"));
		}else{
			next1=0;
			next2=0;
			if(frame>=0){
				preview1.loadImage(ofFilePath::join(captures1.getAbsolutePath(),ofToString(frame)+".png"));
				preview2.loadImage(ofFilePath::join(captures2.getAbsolutePath(),ofToString(frame)+".png"));
			}
			frame++;
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(ofWindowEventArgs & window){
	ofSetColor(255);
	if(window.window->getID()!=0){
		if(!end){
			ofPushMatrix();
			ofScale(scale,scale);
			output.draw(0,0);
			ofPopMatrix();
		}
	}else{
		if(!showDecoder){
			grabber1.draw(0,0,640,imageHeight);
			grabber2.draw(640,0,640,imageHeight);
		}else if(end){
			decoder1.draw(0,imageHeight,640,imageHeight);
			decoder2.draw(640,imageHeight,640,imageHeight);
		}
		if(preview1.isAllocated() && preview2.isAllocated()){
			preview1.draw(0,imageHeight,640,imageHeight);
			preview1.draw(640,imageHeight,640,imageHeight);
		}
		gui.draw();
		ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='+' && end){
		frame++;
		frame%=encoder.getFrameCount();
		preview1.loadImage(ofFilePath::join(captures1.getAbsolutePath(),ofToString(frame)+".png"));
		preview2.loadImage(ofFilePath::join(captures2.getAbsolutePath(),ofToString(frame)+".png"));
	}
	if(key==' ' && !start){
		start=true;
		ofPtr<ofWindow> window = ofWindow::createWindow(width,height,OF_FULLSCREEN,1);
		window->addListener(this);
		window->setup();
	}
	if(key=='d'){
		showDecoder = !showDecoder;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
