#include "SketchShareView.h"


void SketchShareView::initViewer()
{
	camWidth_l 		= 556; // try to grab at this size. 
	camHeight_l 	= 415;
	camWidth_s 		= 200;
	camHeight_s 	= 150;
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(640,480);

	bgSmall.loadImage("images/smallerViewer.png");
	bgBig.loadImage("images/biggerViewer.png");
	
	zoomIn=false;
	close=false;
	//isClosed=1;

	bgX=0;
	bgY=0;
}

void SketchShareView::update(){
	vidGrabber.grabFrame();
}

void SketchShareView::drawBg(){
	
		if(!zoomIn){
			bgX=100;
			bgY=100;
			bgSmall.draw(bgX,bgY);
		} else {
			bgX=20;
			bgY=20;
			bgBig.draw(bgX,bgY);
		}
	
}

void SketchShareView::drawVideo(){
	
		ofPushMatrix();
	
		ofTranslate(bgX,bgY);
		ofSetColor(0xffffff);
		if(!zoomIn){
			vidGrabber.draw(9,36,camWidth_s,camHeight_s);
		} else {
			vidGrabber.draw(43,40,camWidth_l,camHeight_l);
		}

		ofPopMatrix();
	
}

void SketchShareView::zoomDetect(int mouseX, int mouseY){

	
	if(!zoomIn){
		if((mouseX>bgX+138 && mouseX<bgX+138+28)&&(mouseY>bgY+4 && mouseY<bgY+4+28)) zoomIn=true;
		
	} else {
		if((mouseX>bgX+530)&&(mouseX<bgX+530+28)&&(mouseY>bgY+6)&&(mouseY<bgY+6+28)) zoomIn=false;
	}
	

}

void SketchShareView::closeDetect(int mouseX, int mouseY){	
	if(!zoomIn){
		if((mouseX>bgX+176 && mouseX<bgX+176+28)&&(mouseY>bgY+4 && mouseY<bgY+4+28)) close=true;
	} else {
		if((mouseX>bgX+568)&&(mouseX<bgX+568+28)&&(mouseY>bgY+6)&&(mouseY<bgY+6+28)) close=true;
	}
}


