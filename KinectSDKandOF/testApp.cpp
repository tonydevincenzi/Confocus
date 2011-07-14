#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(255,255,255);	
	
	//blur.setup(640, 480);
	//hasCamera = grabber.initGrabber(640, 480);
	
	
	printf("initializing\n");
	g_videoGrabber.Video_Zero();
	g_videoGrabber.Video_Init();
	//printf("gathering data\n");

	texColorAlpha.allocate(VIDEO_WIDTH,VIDEO_HEIGHT,GL_RGBA);
	//colorAlphaPixels1	= new unsigned char [WIDTH*HEIGHT*4];
	texGray.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); // gray depth texture
}

//--------------------------------------------------------------
void testApp::update(){
	//ofSetWindowTitle(ofToString(ofGetFrameRate(), 2.0));
	//grabber.update();
	
	g_videoGrabber.Video_Update();
	//g_videoGrabber.print_bytes();
	//printf("loading data\n");
	colorAlphaPixels = g_videoGrabber.getAlphaPixels();
	grayPixels = (BYTE*)g_videoGrabber.Kinect_getDepthPixels();	
	if(colorAlphaPixels != NULL) {
		//printf("%d %d %d %d \n", colorAlphaPixels[0],colorAlphaPixels[1],colorAlphaPixels[2],colorAlphaPixels[3]);
		texColorAlpha.loadData(colorAlphaPixels, VIDEO_WIDTH,VIDEO_HEIGHT, GL_RGBA);	
	}
	if (grayPixels != NULL) {
		texGray.loadData(grayPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	}
	//int n= g_videoGrabber.getJointsPixels();
	//printf("%d\n",n);
	
	g_videoGrabber.getJointsPoints();
	headPositionX=g_videoGrabber.headJoints_x;
	headPositionY=g_videoGrabber.headJoints_y;
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
	g_videoGrabber.Video_UnInit();
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

