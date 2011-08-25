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

	ofImage             bgSmall, bgBig;
	int                 bgX, bgY;
	
	bool                zoomIn;
	bool                close;
	//int                 isClosed;
	//SketchShareView();
	
	void initViewer();
	void update();
	void drawBg();
	void drawVideo();
	void zoomDetect(int mouseX, int mouseY);
	void closeDetect(int mouseX, int mouseY);
	//void switchMode(int x);
};

