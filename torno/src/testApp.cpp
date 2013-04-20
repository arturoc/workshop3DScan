#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	serial.setup("/dev/ttyUSB0",9600);
	kinect.init();
	kinect.open();
	kinect.setRegistration(true);

	stepsOneTurn = 200;
	steps = 10;
	numTurns = 0;


	center.setMode(OF_PRIMITIVE_POINTS);
	center.addVertex(ofVec3f(0,0,0));

	addMesh.setMode(OF_PRIMITIVE_POINTS);

	selectionQuad.setMode(OF_PRIMITIVE_LINE_LOOP);
	selectionQuad.addVertex(ofVec3f(-100,0,-100));
	selectionQuad.addVertex(ofVec3f(100,0,-100));
	selectionQuad.addVertex(ofVec3f(100,0,100));
	selectionQuad.addVertex(ofVec3f(-100,0,100));

	mesh.getVertices().resize(640*480);
	mesh.getTexCoords().resize(640*480);
	mesh.setMode(OF_PRIMITIVE_POINTS);
	ofBackground(0);
	for(int y=0;y<480;y++){
		for(int x=0;x<640;x++){
			mesh.getVertices()[y*640+x].set(x,y);
			mesh.getTexCoords()[y*640+x].set(x,y);
		}
	}

	gui.setup();
	//gui.add(nearClip.set("near",1,1,100));
	//gui.add(farClip.set("far",100,1,500));
	gui.add(camtopPos.set("camtopPos",ofVec3f(0,-1095,614),ofVec3f(-500,-1200,-1000),ofVec3f(500,0,1000)));
	gui.add(topLookAt.set("topLookAt",ofVec3f(0,0,1),ofVec3f(-1,-1,-1),ofVec3f(1,1,1)));
	gui.add(topUp.set("topUp",ofVec3f(0,-1,0),ofVec3f(-1,-1,-1),ofVec3f(1,1,1)));
	gui.add(selectionSide.set("selectionSide",100,50,500));
	gui.add(angle.set("angle",0,0,360));
	gui.add(triMeshThreshold.set("triMeshThreshold",100,10,1000));

	selectionSide.addListener(this,&testApp::selectionSideChanged);

	gui.loadFromFile("settings.xml");

	camFront.setFov(60);
	camFront.move(0,0,0);
	camFront.lookAt(ofVec3f(0,0,1),ofVec3f(0,-1,0));
	camTop.setFov(60);
	//camTop.move(0,100,50);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
}

void testApp::selectionSideChanged(int & side){
	selectionQuad.clear();
	selectionQuad.addVertex(ofVec3f(-side,0,-side));
	selectionQuad.addVertex(ofVec3f(side,0,-side));
	selectionQuad.addVertex(ofVec3f(side,0,side));
	selectionQuad.addVertex(ofVec3f(-side,0,side));
}

//--------------------------------------------------------------
void testApp::update(){
	while(serial.available()>0){
		memset(buffer,0,sizeof(buffer));
		serial.readBytes(buffer,1024);
		cout << buffer;
	}
	kinect.update();
	if(kinect.isFrameNew()){
		int i=0;
		for(int y=0;y<480;y++){
			for(int x=0;x<640;x++,i++){
				mesh.getVertices()[i] = kinect.getWorldCoordinateAt(x,y);
				/*mesh.getColors()[i].set(kinect.getPixelsRef()[i*3],
										kinect.getPixelsRef()[i*3+1],
										kinect.getPixelsRef()[i*3+2]);*/
			}
		}


		triMesh.clear();
		triMesh = mesh;
		for(int y=0;y<480-1;y++){
			for(int x=0;x<640-1;x++,i++){
				ofVec3f v1 = mesh.getVertex(y*640+x);
				ofVec3f v2 = mesh.getVertex((y+1)*640+x);
				ofVec3f v3 = mesh.getVertex((y+1)*640+(x+1));
				if(v1.squareDistance(v2)<triMeshThreshold &&
				   v2.squareDistance(v3)<triMeshThreshold &&
				   v1.squareDistance(v3)<triMeshThreshold ){
					triMesh.addIndex(y*640+x);
					triMesh.addIndex((y+1)*640+x);
					triMesh.addIndex((y+1)*640+(x+1));

				}
				v1 = mesh.getVertex(y*640+x);
				v2 = mesh.getVertex(y*640+(x+1));
				v3 = mesh.getVertex((y+1)*640+(x+1));
				if(v1.squareDistance(v2)<triMeshThreshold &&
				   v2.squareDistance(v3)<triMeshThreshold &&
				   v1.squareDistance(v3)<triMeshThreshold ){
					triMesh.addIndex(y*640+x);
					triMesh.addIndex(y*640+(x+1));
					triMesh.addIndex((y+1)*640+(x+1));

				}
			}
		}
	}


	camTop.resetTransform();
	camTop.move(camtopPos);
	ofVec3f lookAt(camtopPos->x+topLookAt->x,camtopPos->y+topLookAt->y,camtopPos->z+topLookAt->z);
	camTop.lookAt(lookAt,topUp);
	//cam.setNearClip(nearClip);
	//cam.setFarClip(farClip);
}

//--------------------------------------------------------------
void testApp::draw(){
	glEnable(GL_DEPTH_TEST);
	ofNoFill();
	ofRectangle viewportFront(0,240,640,480);
	kinect.drawDepth(gui.getWidth()+20,0,320,240);
	kinect.draw(gui.getWidth()+20+320,0,320,240);
	camFront.begin(viewportFront);
	kinect.getTextureReference().bind();
	triMesh.draw();
	kinect.getTextureReference().unbind();
	glDisable(GL_DEPTH_TEST);
	selectionQuad.draw();
	ofSetColor(255,0,0);
	glPointSize(3);
	center.draw();
	glPointSize(1);
	camFront.end();
	ofSetColor(255);
	ofRect(viewportFront);
	ofDrawBitmapString("front",viewportFront.getPosition()+ofVec2f(20,20));

	glEnable(GL_DEPTH_TEST);
	ofRectangle viewportTop(640,240,640,480);
	camTop.begin(viewportTop);
	kinect.getTextureReference().bind();
	mesh.draw();
	kinect.getTextureReference().unbind();
	glDisable(GL_DEPTH_TEST);
	ofTranslate(camtopPos->x,0,camtopPos->z);
	selectionQuad.draw();
	ofSetColor(255,0,0);
	glPointSize(3);
	center.draw();
	glPointSize(1);
	camTop.end();
	ofSetColor(255);
	ofRect(viewportTop);
	ofDrawBitmapString("top",viewportTop.getPosition()+ofVec2f(20,20));


	glDisable(GL_DEPTH_TEST);
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	cout << "key pressed" << endl;
	if(key==' '){
		rotationMat.makeIdentityMatrix();
		rotationMat.rotate(numTurns*steps*360./float(stepsOneTurn),0,1,0);
		ofVec3f center(camtopPos->x,0,camtopPos->z);
		for(int i=0;i<(int)mesh.getVertices().size();i++){
			ofVec3f v = mesh.getVertex(i)-center;
			if(v.x>-selectionSide && v.x<selectionSide && v.z>-selectionSide && v.z<selectionSide) {
				v = rotationMat*v;
				/*bool found=false;
				for(int j=0;j<addMesh.getVertices().size();j++){
					if(v.squareDistance(addMesh.getVertex(j))<10){
						found =true;
						break;
					}
				}*/
				//if(!found)
					addMesh.addVertex(v);
			}
		}

		//camFront.setTarget(addMesh.getCentroid());
		numTurns++;
		serial.writeByte((unsigned char)steps);
		angle = numTurns*steps*360./float(stepsOneTurn);
	}
	if(key=='s'){
		addMesh.save("mesh.ply");
		triMesh.save("triMesh.ply");
	}

	if(key==OF_KEY_LEFT){
		camtopPos = ofVec3f(camtopPos->x-1,camtopPos->y,camtopPos->z);
	}
	if(key==OF_KEY_RIGHT){
		camtopPos = ofVec3f(camtopPos->x+1,camtopPos->y,camtopPos->z);
	}
	if(key==OF_KEY_UP){
		camtopPos = ofVec3f(camtopPos->x,camtopPos->y,camtopPos->z+1);
	}
	if(key==OF_KEY_DOWN){
		camtopPos = ofVec3f(camtopPos->x,camtopPos->y,camtopPos->z-1);
	}
	if(key=='r'){
		addMesh.clear();
		serial.writeByte((unsigned char)stepsOneTurn-numTurns*steps);
		numTurns=0;
		angle = 0;
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
