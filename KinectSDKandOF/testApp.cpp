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
	texHighlight.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); 
}

//--------------------------------------------------------------
void testApp::update(){
	//ofSetWindowTitle(ofToString(ofGetFrameRate(), 2.0));
	//grabber.update();
	
	g_kinectGrabber.Kinect_Update();
	
	colorAlphaPixels = g_kinectGrabber.Kinect_getAlphaPixels();
	if(colorAlphaPixels != NULL) {
		texColorAlpha.loadData(colorAlphaPixels, VIDEO_WIDTH,VIDEO_HEIGHT, GL_RGBA);	
	}

	grayPixels = (BYTE*)g_kinectGrabber.Kinect_getDepthPixels();	
	if (grayPixels != NULL) {
		texGray.loadData(grayPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	}

	highlightPixels = highlightRGB(colorAlphaPixels, g_kinectGrabber.Kinect_getPlayerBuffer());
	if(highlightPixels != NULL) {
		texHighlight.loadData(highlightPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);	
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
	//texGray.draw(0,0,DEPTH_WIDTH,DEPTH_HEIGHT);
	texHighlight.draw(0,0,DEPTH_WIDTH,DEPTH_HEIGHT);
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


//TODO: move this into a "features" file
BYTE* testApp::highlightRGB(BYTE* videoBuff, USHORT* playerBuff) {
	  BYTE * highlightBuff = new unsigned char [DEPTH_WIDTH*DEPTH_HEIGHT*4];
	  if (videoBuff && playerBuff) {
		  for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
				for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
					//if that pixel does not belong to a player,  black it out
					if (!playerBuff[y*DEPTH_WIDTH + x]) {
						highlightBuff[4*((y*DEPTH_WIDTH) + x) + 0] = 0;
						highlightBuff[4*((y*DEPTH_WIDTH) + x) + 1] = 0;
						highlightBuff[4*((y*DEPTH_WIDTH) + x) + 2] = 0;
						highlightBuff[4*((y*DEPTH_WIDTH) + x) + 3] = 255;
					
					} else {
						highlightBuff[4*((y*DEPTH_WIDTH) + x) + 0] = videoBuff[4*((2*y*VIDEO_WIDTH) + (2*x)) + 0];
						highlightBuff[4*((y*DEPTH_WIDTH) + x) + 1] = videoBuff[4*((2*y*VIDEO_WIDTH) + (2*x)) + 1];
						highlightBuff[4*((y*DEPTH_WIDTH) + x) + 2] = videoBuff[4*((2*y*VIDEO_WIDTH) + (2*x)) + 2];
						highlightBuff[4*((y*DEPTH_WIDTH) + x) + 3] = 255;
					
					}
				}
		  }
	  }
	  return highlightBuff;
}