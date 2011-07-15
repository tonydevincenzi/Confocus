#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(0,0,0);	
	
	//blur.setup(640, 480);
	//hasCamera = grabber.initGrabber(640, 480);
	
	
	printf("initializing\n");
	g_kinectGrabber.Kinect_Zero();
	g_kinectGrabber.Kinect_Init();
	//printf("gathering data\n");

	texColorAlpha.allocate(VIDEO_WIDTH,VIDEO_HEIGHT,GL_RGBA);
	//colorAlphaPixels1	= new unsigned char [WIDTH*HEIGHT*4];
	texGray.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); // gray depth texture
}

//--------------------------------------------------------------
void testApp::update(){
	//ofSetWindowTitle(ofToString(ofGetFrameRate(), 2.0));
	//grabber.update();
	
	g_kinectGrabber.Kinect_Update();
	
	colorAlphaPixels = g_kinectGrabber.getAlphaPixels();
	if(colorAlphaPixels != NULL) {
		texColorAlpha.loadData(colorAlphaPixels, VIDEO_WIDTH,VIDEO_HEIGHT, GL_RGBA);	
	}
	
	grayPixels = (BYTE*)g_kinectGrabber.Kinect_getDepthPixels();	
	if (grayPixels != NULL) {
		texGray.loadData(grayPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	}
	
	//int n= g_kinectGrabber.getJointsPixels();
	//printf("%d\n",n);
	
	g_kinectGrabber.getJointsPoints();
	headPositionX=g_kinectGrabber.headJoints_x;
	headPositionY=g_kinectGrabber.headJoints_y;
	//printf("position: %d\n",headPositionX);
	

}

//--------------------------------------------------------------
void testApp::draw(){
	//printf("drawing image\n");
	texColorAlpha.draw(0,0,VIDEO_WIDTH,VIDEO_HEIGHT);
	texGray.draw(0,0,DEPTH_WIDTH,DEPTH_HEIGHT);
	ofCircle(headPositionX,headPositionY,20);
	/*
	blur.setBlurParams(4, (float)mouseX / 100.0);
	blur.beginRender();
		
		if( hasCamera ){
			ofSetColor(255, 255, 255);
			grabber.draw(0, 0);
			
			ofSetColor(200, 10, 20);
			ofCircle(ofGetWidth() - 100, 200, 50);
		}else{
			ofSetColor(200, 10, 20);
			ofCircle(ofGetWidth()/2, ofGetHeight()/2, 100);		
		}
		
	blur.endRender();
	blur.draw(0, 0, 640, 480, true);
	
	ofDrawBitmapString("move mouse to control blur", 20, 20);
	*/
}
//-------------------------------------------------------------
void testApp::exit(){
	printf("cleaning up\n");
	g_kinectGrabber.Kinect_UnInit();
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//------------- -------------------------------------------------
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

