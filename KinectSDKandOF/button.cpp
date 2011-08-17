#include "ofMain.h"
#include "button.h"



button::button(string _buttonName, int _originX, int _originY, int _bWidth, int _bHeight, bool _trigger)
{
	buttonName=_buttonName;	
	fontSize=16;
	font.loadFont("Sudbury_Basin_3D.ttf", fontSize);
	//font.loadFont("castaway.ttf",32);

	originX=_originX;
	originY=_originY;
	bWidth=_bWidth;
	bHeight=_bHeight;
	trigger=_trigger;

	
}

void button::drawFont(bool _buttonPressed){
	ofPushMatrix();
	if (_buttonPressed) ofSetColor(0xFF9933);
	else ofSetColor(0xCCCCCC);
	font.drawString(buttonName, originX,originY-10);
	
	ofNoFill();
	ofRect(originX, originY-bHeight,bWidth,bHeight);
	ofPopMatrix();
}

bool button::buttonPressed(int x, int y){
		if((x>=originX && x<originX+bWidth) && (y>=originY-bHeight && y<originY) && trigger){
			return true;
		}else{
			return false;
		}
}
