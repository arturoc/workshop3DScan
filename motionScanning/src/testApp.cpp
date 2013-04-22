#include "testApp.h"
#include "proc_info.h"

//--------------------------------------------------------------
void testApp::setup()
{
    kinect.init(false);
    kinect.open();
    kinect.setRegistration(true);
    //mesh.getVertices().resize(640*480);
    mesh.setMode(OF_PRIMITIVE_POINTS);
    //mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    camera.setFov(60);
    camera.move(0,0,0);
    camera.lookAt(ofVec3f(0,0,1),ofVec3f(0,-1,0));
}

//--------------------------------------------------------------
void testApp::update()
{
    kinect.update();
    //mesh.clear();
    if(bRec)
    {
        int sampling = 4;
        int sample = 0;

        kinect.getDistancePixelsRef();
        for(int y = 0; y < 480; y++)
        {
            for(int x = 0; x < 640; x++)
            {
                if(sample++%sampling == 0)
                {
                    ofVec3f v = kinect.getWorldCoordinateAt(x, y);
                    if(v.z < zThreshold)
                    {
                        //mesh.addVertex(v);
                        //v.z = -v.z;
                        //v.z=v.z+2*v.distance(ofVec3f(0,0,0));
                        mesh.addVertex(v);
                        mesh.addTexCoord(ofVec2f(x,y));
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::draw()
{

    ofBackground(0);
    //kinect.drawDepth(1024,480);

    //ofSetColor(255,0,0);
    glEnable(GL_DEPTH_TEST);
    camera.begin();
    //kinect.getTextureReference().bind();
    mesh.draw();
    //kinect.getTextureReference().unbind();
    camera.end();
    glDisable(GL_DEPTH_TEST);

// finally, a report:

    double vm, rss;
    process_mem_usage(vm, rss);

    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "3D reconstruction with the kinect" << endl
              << ", fps: " << ofGetFrameRate() << endl
              << "VM: " << vm << "; RSS: " << rss << endl;
    ofDrawBitmapString(reportStr.str(), 660, 20);

}

//--------------------------------------------------------------

void testApp::keyPressed(int key)
{
    //camera.setTarget(ofVec3f(0,0,0));
}

void testApp::keyReleased(int key)
{
    cout << "key pressed: " << key << endl;
    switch (key)
    {
    case ' ':
        break;
    case OF_KEY_LEFT:
        break;
    case OF_KEY_RIGHT:
        break;
    case 's':
        mesh.save("mesh.ply");
        break;
    case 'c':
        mesh.clear();
        break;
    case 'r':
        bRec=!bRec;
        break;
    }
}

void testApp::mouseReleased(int x, int y, int button)
{

    switch(button)
    {
    case 0:
        break;
    case 2:
        break;
    }


}

//void testApp::scanDevices()
//{
//    int i = 0,  deviceID = 0;;
//    do
//    {
//        ofGstVideoGrabber* g = new ofGstVideoGrabber;
//        g->setDeviceID(i);
//        deviceID = g->deviceID;
//        ofVideoGrabber* vg = (new ofVideoGrabber);
//        vg->setGrabber(ofPtr<ofGstVideoGrabber>(g));
//        Capture* c = new Capture(deviceID, g->camData.webcam_devices[deviceID].product_name, *vg);
//        capturesAvailable.push_back(c);
//    }
//    while(i++ <= deviceID);
//
//    // last device is a duplicate, deallocate it
//    Capture* c = capturesAvailable.back();
//    capturesAvailable.pop_back();
//    delete &(c->vidGrabber);
//    delete c;
//
//}
