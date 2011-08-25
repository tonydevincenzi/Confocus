#pragma once
#include "ofMain.h"

class SketchShareView
{
public:
	
	ofVideoGrabber 		vidGrabber;
	ofTexture			videoTexture;
	
	int 				camWidth_s;
	int 				camHeight_s;
	int                 camWidth_l;
	int                 camHeight_l;
	
	//SketchShareView();
	
	void initViewer();
	void update();
	void draw();
};

