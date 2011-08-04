#include "testApp.h"
#include <algorithm>

//--------------------------------------------------------------
void testApp::setup(){	

	ofEnableAlphaBlending();
	
	ofBackground(255,255,255);	
	
	blur.setup(DEPTH_WIDTH, DEPTH_HEIGHT);
	thresh=300;

	g_kinectGrabber.Kinect_Zero();
	g_kinectGrabber.Kinect_Init();
	conference_init();
	
	focusPixels = new unsigned char [DEPTH_WIDTH*DEPTH_HEIGHT*4];
	blurPixels = new unsigned char [DEPTH_WIDTH*DEPTH_HEIGHT*4];

	texColorAlpha.allocate(VIDEO_WIDTH,VIDEO_HEIGHT,GL_RGBA);
	texFocus.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); 
	texBlur.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); 
	//blurImg.allocate(DEPTH_WIDTH, DEPTH_HEIGHT);

	//texGray.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); // gray depth texture	

	activeFocus=true;
}

//--------------------------------------------------------------
void testApp::update(){
	
	g_kinectGrabber.Kinect_Update();
	//conference_update();

	colorAlphaPixels = g_kinectGrabber.Kinect_getAlphaPixels();
	if(colorAlphaPixels != NULL) {
		texColorAlpha.loadData(colorAlphaPixels, VIDEO_WIDTH,VIDEO_HEIGHT, GL_RGBA);
	}
	/*
	grayPixels = (BYTE*)g_kinectGrabber.Kinect_getDepthPixels();	
	if (grayPixels != NULL) {
		texGray.loadData(grayPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	}*/
	
	USHORT* depthBuff = g_kinectGrabber.Kinect_getDepthBuffer();
	focusRGB(colorAlphaPixels, depthBuff, focusPixels, blurPixels, &g_kinectGrabber,mouseX,mouseY, activeFocus);
	
	/*
	if(focusPixels != NULL) {
		
		adjustOver(2, blurPixels);
		texBlur.loadData(blurPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	}
	*/

	texFocus.loadData(focusPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	texBlur.loadData(blurPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	//blurImg.setFromPixels(blurPixels,DEPTH_WIDTH, DEPTH_HEIGHT);
	
	
	//int n= g_kinectGrabber.getJointsPixels();
	//printf("%d\n",n);
	
	g_kinectGrabber.getJointsPoints();
	headPositionX=g_kinectGrabber.headJoints_x;
	headPositionY=g_kinectGrabber.headJoints_y;
	headPositionZ=g_kinectGrabber.headJoints_z;
	//printf("position: %d\n",headPositionX);
	

}

//--------------------------------------------------------------
void testApp::draw(){

	//video image
	texColorAlpha.draw(0,0,VIDEO_WIDTH, VIDEO_HEIGHT);

	ofEnableAlphaBlending();
	//diminished image
	//blurImg.blurGaussian(15);
	//blurImg.draw(640,0);
	texFocus.draw(640,0,DEPTH_WIDTH, DEPTH_HEIGHT); //draw the focus texture	
	//blur.setBlurParams(4, (float)mouseX / 100.0); //Gaussian blur starts
	blur.setBlurParams(4,(float)100/100);
	blur.beginRender();
	texBlur.draw(0,0,DEPTH_WIDTH, DEPTH_HEIGHT);
	blur.endRender();
	blur.draw(640, 0, 640, 480, true);
	ofDisableAlphaBlending();
	//texGray.draw(640,0,DEPTH_WIDTH,DEPTH_HEIGHT);

	ofCircle(headPositionX,headPositionY,20);

	ofSetColor(0x00000);
	//char reportStr[1024];
	//sprintf(reportStr, " threshold: %d (press: +/- to change) \n", thresh);
	ofDrawBitmapString(eventString, 650, 500);
	ofSetColor(0xffffff);
}
//-------------------------------------------------------------
void testApp::exit(){
	printf("cleaning up\n");
	g_kinectGrabber.Kinect_UnInit();
	free(focusPixels);
	free(blurPixels);
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	/*switch (key){
		case '+':
			thresh += 20;
			if (thresh > 1000) thresh = 1000;
			break;
		case '-':
			thresh -= 20;
			if (thresh < 10) thresh = 10;
			break;
	}*/
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//------------- -------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	sprintf(eventString, "mouseMoved = (%i,%i)", x, y);
		
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	activeFocus=!activeFocus;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
