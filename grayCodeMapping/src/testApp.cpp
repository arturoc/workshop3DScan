#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	string path = "";
	payload.init(1024, 768);
	encoder.init(payload);



	decoder1.init(payload);
	for(int i=0;i<encoder.getFrameCount();i++){
		imgColor.loadImage(path+"capture1/"+ofToString(i)+".png");
		decoder1 << imgColor;
	}


	decoder2.init(payload);
	for(int i=0;i<encoder.getFrameCount();i++){
		imgColor.loadImage(path+"capture2/"+ofToString(i)+".png");
		decoder2 << imgColor;
	}

	ofEnableAlphaBlending();
	ofBackground(255);

	imgColor.loadImage(path+"capture2/1.png");
	mesh.setMode(OF_PRIMITIVE_POINTS);
	meshMapping.setMode(OF_PRIMITIVE_POINTS);

	float zNear = -1;
	float zFar = 1000;
	float cx = 6.7843858886797614e+02;
	float cy = 3.4517466503352483e+02;
	float fx = 1.0719532879358244e+03;
	float fy = 1.0703376726883023e+03;
	float w = 1280;
	float h = 720;

	ofxRay::Camera camera1(ofVec2f(fx,fy),ofVec2f(cx,cy),w,h);
	camera1.setDefaultNear(zNear);
	camera1.setDefaultFar(zFar);
	ofxRay::Camera camera2(ofVec2f(fx,fy),ofVec2f(cx,cy),w,h);
	camera2.setDefaultNear(zNear);
	camera2.setDefaultFar(zFar);
	camera1.move(-69,0,0);
	camera2.move(69,0,0);

	videoMapping.loadMovie("mapping.mp4");
	videoMapping.play();
	videoMapping.setLoopState(OF_LOOP_NORMAL);

	ofVec3f world ;
	for(int i=0;i<decoder1.getDataSet().getDataInverse().size();i++){
		int d1 = decoder1.getDataSet().getDataInverse()[i];
		int d2 = decoder2.getDataSet().getDataInverse()[i];
		int x = i%decoder1.getDataSet().getDataInverse().getWidth();
		int y = i/decoder1.getDataSet().getDataInverse().getWidth();
		int d2_x = d2%decoder1.getDataSet().getData().getWidth();
		int d2_y = d2/decoder2.getDataSet().getData().getWidth();
		if(decoder1.getDataSet().getActive()[d1] && decoder2.getDataSet().getActive()[d2]){
			world = ofxTriangulate::Triangulate(d1,d2,camera1,camera2);
			mesh.addVertex(world);
			mesh.addTexCoord(ofVec2f(d2_x,d2_y));

			meshMapping.addVertex(world);
			meshMapping.addColor(imgColor.getPixelsRef().getColor(d2_x,d2_y));
			meshMapping.addTexCoord(ofVec2f(x,y));
		}
	}

	mesh.save("mesh.ply");

	ofVec3f centroid=mesh.getCentroid();

	camera.setTarget(centroid);

	//glEnable(GL_POINT_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glPointSize(2);
	glEnable(GL_DEPTH_TEST);

	mode = ThreeD;
}

//--------------------------------------------------------------
void testApp::update(){
	videoMapping.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	switch(mode){
	case ThreeD:
		camera.begin();
		imgColor.bind();
		mesh.draw();
		imgColor.unbind();
		camera.end();
		break;
	case Mapping:
		camera.begin();
		videoMapping.getTextureReference().bind();
		meshMapping.draw();
		videoMapping.getTextureReference().unbind();
		camera.end();
		break;
	}
	videoMapping.draw(0,0,320,240);
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' '){
		mode= (Mode)(int(mode)+ 1);
		mode= (Mode)(int(mode)%2);
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
