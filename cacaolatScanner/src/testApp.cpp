#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
	#else
        vidPlayer.loadMovie("video.mov");
		vidPlayer.setLoopState(OF_LOOP_NONE);
        vidPlayer.play();
	#endif

    colorImg.allocate(1280,720);
	grayImage.allocate(1280,720);

	grayBg.allocate(1280,720);
	grayDiff.allocate(1280,720);
	fbo.allocate(1280,720,GL_LUMINANCE);
	pixelsDepth.allocate(1280,720,OF_IMAGE_GRAYSCALE);
	pixelsOneFrame.allocate(1280,720,OF_IMAGE_GRAYSCALE);
	pixelsDepth.getPixelsRef().set(0);
	tmp.allocate(1280,720,OF_IMAGE_GRAYSCALE);
	
	printf("%i,%i",vidPlayer.width, vidPlayer.height);

	bLearnBakground = true;
	threshold = 65;
	bPause = false;
	bVer = false;
	bFill = false;
	bOn=false;

	mesh.setMode(OF_PRIMITIVE_POINTS);

}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 1280,720);
        #endif

        grayImage = colorImg;
		
		grayDiff = grayImage;
		grayDiff.threshold(threshold);
		grayDiff.erode();
		grayDiff.dilate();
		
		grayDiff.dilate();
		grayDiff.erode();
		grayDiff.erode();
		grayDiff.erode();
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (1280*720)/3, 3,false);	// find holes
		/*
		fbo.begin();
		ofClear(0);
		ofFill();  
        ofSetHexColor(0xFFFFFF);  
        ofBeginShape();  
        for (int i = 0; i < contourFinder.blobs[2].nPts; i++){  
                ofVertex(contourFinder.blobs[2].pts[i].x, contourFinder.blobs[2].pts[i].y);  
        }  
        ofEndShape(true);  
		fbo.end();
		fbo.readToPixels(pixelsOneFrame);

		*/
		if (bOn)
		{
			//cvAnd(grayDiff.getCvImage(), pixelsOneFrame.getPixels(), tmp.getPixels());
			for(int i=0;i<pixelsDepth.getPixelsRef().size();i++){
				if (grayDiff.getPixelsRef()[i] && pixelsOneFrame.getPixelsRef()[i])
				//if (grayDiff.getPixelsRef()[i])
					pixelsDepth.getPixelsRef()[i]++;
			}
			pixelsDepth.update();
		}
		//pixelsOneFrame.update();
		
		
	}


}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xffffff);
	
	if (bVer)
	{


		if (bFill)
		{
		
            ofFill();  
            ofSetHexColor(0xFFFFFF);  
            ofBeginShape();  
            for (int i = 0; i < contourFinder.blobs[2].nPts; i++){  
                   ofVertex(contourFinder.blobs[2].pts[i].x, contourFinder.blobs[2].pts[i].y);  
            }  
            ofEndShape(true);   
		}
		else
		{
			grayDiff.draw(0,0);
			for (int i = 0; i < contourFinder.nBlobs; i++)
				contourFinder.blobs[i].draw(0,0);
		}

	}
	else
	{
		// draw the incoming, the grayscale, the bg and the thresholded difference

		colorImg.draw(20,20,500,250);
		//grayImage.draw(20,255,500,250);
		grayDiff.draw(20,255,500,250);
		pixelsOneFrame.draw(500,20,500,250);
		pixelsDepth.draw(500,255,500,250);
		//fbo.draw(20,20,500,250);

		// then draw the contours:

		ofFill();
		ofSetHexColor(0x333333);
		ofRect(360,540,320,240);
		ofSetHexColor(0xffffff);

		// we could draw the whole contour finder
		//contourFinder.draw(360,540);

		// or, instead we can draw each blob individually,
		// this is how to get access to them:
		
		//for (int i = 0; i < contourFinder.nBlobs; i++){
			//contourFinder.blobs[i].draw(525,0);
    }

	

	// finally, a report:

	ofSetHexColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "threshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
	ofDrawBitmapString("on -> " + ofToString(bOn),20,620);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
	case '1':
		bOn = !bOn;
		break;
	case 'o':
		fbo.begin();
		ofClear(0);
		ofFill();  
        ofSetHexColor(0xFFFFFF);  
        ofBeginShape();  
        for (int i = 0; i < contourFinder.blobs[0].nPts; i++){  
                ofVertex(contourFinder.blobs[0].pts[i].x, contourFinder.blobs[0].pts[i].y);  
        }  
        ofEndShape(true);  
		fbo.end();
		fbo.readToPixels(pixelsOneFrame);
		pixelsOneFrame.update();
		break;

		case ' ':
			bPause = !bPause;
			vidPlayer.setPaused(bPause);
			break;
		case 'f':
			bFill = !bFill;
			break;

		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case 'v':
			bVer = !bVer;
			break;
		case 's':
			pixelsDepth.saveImage("depth.png");
			int max = 0;
			for(int i=0;i<pixelsDepth.getPixelsRef().size();i++){
				if(max<pixelsDepth.getPixelsRef()[i]){
					max=pixelsDepth.getPixelsRef()[i];
				}
			}
			cout << max << endl;
			saveMesh();
			break;
	}
}

void testApp::saveMesh()
{

	for(int y=0;y<720;y++)
		for(int x=0;x<1280;x++)
		{
			ofVec3f v = ofVec3f(x,y,pixelsDepth.getPixelsRef()[y*1280+x]);
			if (v.z)
				mesh.addVertex(v);
			//mesh.addTexCoord(ofVec2f(x,y));
		}
	mesh.save("mesh.ply");
	mesh.clear();
		for(int y=0;y<720;y++)
		for(int x=0;x<1280;x++)
		{
			ofVec3f v = ofVec3f(x,y,pixelsDepth.getPixelsRef()[y*1280+x]);
			v.z = v.z/4;
			if (v.z)
				mesh.addVertex(v);
			//mesh.addTexCoord(ofVec2f(x,y));
		}
	mesh.save("mesh2.ply");
	mesh.clear();
			for(int y=0;y<720;y++)
		for(int x=0;x<1280;x++)
		{
			ofVec3f v = ofVec3f(x,y,pixelsDepth.getPixelsRef()[y*1280+x]);
			v.z = v.z/5;
			if (v.z)
				mesh.addVertex(v);
			//mesh.addTexCoord(ofVec2f(x,y));
		}
	mesh.save("mesh3.ply");
	mesh.clear();
				for(int y=0;y<720;y++)
		for(int x=0;x<1280;x++)
		{
			ofVec3f v = ofVec3f(x,y,pixelsDepth.getPixelsRef()[y*1280+x]);
			v.z = v.z/6;
			if (v.z)
				mesh.addVertex(v);
			//mesh.addTexCoord(ofVec2f(x,y));
		}
	mesh.save("mesh4.ply");


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
