#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(255,255,255);	
	
	blur.setup(640, 480);
	hasCamera = grabber.initGrabber(640, 480);
	
	imageWidth = g_videoGrabber.getImageWidth();
	imageHeight = g_videoGrabber.getImageHeight();
	
	printf("initializing\n");
	g_videoGrabber.Video_Zero();
	g_videoGrabber.Video_Init();
	printf("gathering data\n");

	texColorAlpha.allocate(imageWidth,imageHeight,GL_RGBA);
	//colorAlphaPixels1	= new unsigned char [WIDTH*HEIGHT*4];
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 2.0));
	grabber.update();
	
	g_videoGrabber.Video_Update();
	g_videoGrabber.print_bytes();
	printf("loading data\n");
	colorAlphaPixels = g_videoGrabber.getAlphaPixels();
	//printf("%d %d %d %d \n", colorAlphaPixels[0],colorAlphaPixels[1],colorAlphaPixels[2],colorAlphaPixels[3]);
	texColorAlpha.loadData(colorAlphaPixels, imageWidth,imageHeight, GL_RGBA);

}

//--------------------------------------------------------------
void testApp::draw(){
	printf("drawing image\n");
	texColorAlpha.draw(0,0,imageWidth,imageHeight);
	
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

