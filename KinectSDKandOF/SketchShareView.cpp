#include "SketchShareView.h"


void SketchShareView::initViewer()
{
	camWidth_l 		= 602; // try to grab at this size. 
	camHeight_l 	= 451;
	camWidth_s 		= 200;
	camHeight_s 	= 150;
	
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(1);
	vidGrabber.initGrabber(640,480);

	bgSmall.loadImage("images/smallerViewer.png");
	bgBig.loadImage("images/biggerViewer.png");
	
	zoomIn=false;
	close=false;
	//isClosed=1;

	bgX_small=0;
	bgY_small=0;
	bgX_large=0;
	bgY_large=0;
}

void SketchShareView::update(int _xSmall, int _ySmall, int _xLarge, int _yLarge){
	vidGrabber.grabFrame();
	bgX_small=_xSmall;
	bgY_small=_ySmall;
	bgX_large=_xLarge;
	bgY_large=_yLarge;
}

void SketchShareView::drawBg(){
	
		if(!zoomIn){
			bgSmall.draw(bgX_small,bgY_small);
		} else {
			bgBig.draw(bgX_large,bgY_large);
		}
	
}

void SketchShareView::drawVideo(){
	
		ofPushMatrix();
	
		//ofTranslate(bgX,bgY);
		ofSetColor(0xffffff);
		if(!zoomIn){
			ofTranslate(bgX_small, bgY_small);
			vidGrabber.draw(3,36,camWidth_s,camHeight_s);
		} else {
			ofTranslate(bgX_large, bgY_large);
			vidGrabber.draw(35-25,32+22,camWidth_l,camHeight_l);
		}

		ofPopMatrix();
	
}

void SketchShareView::zoomDetect(int mouseX, int mouseY){

	
	if(!zoomIn){
		if((mouseX>bgX_small+176 && mouseX<bgX_small+176+28)&&(mouseY>bgY_small+4 && mouseY<bgY_small+4+28)) zoomIn=true;
		
	} else {
		if((mouseX>bgX_large+568)&&(mouseX<bgX_large+568+28)&&(mouseY>bgY_large+6)&&(mouseY<bgY_large+6+28)) zoomIn=false;
	}
	

}

/*
void SketchShareView::closeDetect(int mouseX, int mouseY){	
	if(!zoomIn){
		if((mouseX>bgX+176 && mouseX<bgX+176+28)&&(mouseY>bgY+4 && mouseY<bgY+4+28)) close=true;
	} else {
		if((mouseX>bgX+568)&&(mouseX<bgX+568+28)&&(mouseY>bgY+6)&&(mouseY<bgY+6+28)) close=true;
	}
}
*/


