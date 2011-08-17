#include "ofMain.h"
#include "slider.h"

slider::slider(int x, int y, int _sWidth)
{
	originX=x;
	originY=y;
	sWidth=_sWidth;
	sliderPosX=x;
	sliderPosY=y;
}

//this function has to be used under void mousePressed() function
void slider::getSliderPosX(int _mouseX, int _mouseY){
	if((_mouseX>=originX && _mouseX<originX+sWidth) && (_mouseY>=originY-8 && _mouseY<originY+5))
		sliderPosX=_mouseX;
}

void slider::drawSlider(int startFrom, int endTo){
	ofFill();
    ofSetColor(0xCCCCCC);
	ofRect(originX,originY,sWidth,5);
	ofSetColor(0xFF9933);
	ofTriangle(sliderPosX,sliderPosY+5,sliderPosX+16,sliderPosY+5,sliderPosX+8,sliderPosY-8);

	value=ofMap(sliderPosX,originX,originX+sWidth,startFrom,endTo);
	//printf("value= %d \n", value);
}


