#include "SketchShareView.h"


void SketchShareView::initViewer()
{
	camWidth_l 		= 640; // try to grab at this size. 
	camHeight_l 	= 480;
	camWidth_s 		= 320;
	camHeight_s 	= 240;
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth_l,camHeight_l);
}

void SketchShareView::update(){
	vidGrabber.grabFrame();
}

void SketchShareView::draw(){
	ofSetColor(0xffffff);
	vidGrabber.draw(20,20,camWidth_s,camHeight_s);
}

