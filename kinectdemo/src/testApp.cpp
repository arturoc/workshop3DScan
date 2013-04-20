#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	kinect.init(false);
	kinect.open();
	kinect.setRegistration(true);
	ofBackground(0);
	mesh.setMode(OF_PRIMITIVE_POINTS);
	camera.setFov(60);
}

//--------------------------------------------------------------
void testApp::update(){
	kinect.update();
	mesh.clear();
	for(int y=0;y<480;y++){
		for(int x=0;x<640;x++){
			ofVec3f v = kinect.getWorldCoordinateAt(x,y);
			mesh.addVertex(v);
			mesh.addTexCoord(ofVec2f(x,y));
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	//kinect.drawDepth(0,0,320,240);
	glEnable(GL_DEPTH_TEST);
	camera.begin();
	kinect.getTextureReference().bind();
	mesh.draw();
	kinect.getTextureReference().unbind();
	camera.end();
	glDisable(GL_DEPTH_TEST);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	camera.setTarget(mesh.getCentroid());
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
