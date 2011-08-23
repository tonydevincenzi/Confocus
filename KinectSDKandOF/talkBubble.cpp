#include "ofMain.h"
#include "talkBubble.h"

talkBubble::talkBubble(float _x, float _y, int _z, int _orientation, string _message, int _talkTime)
{
	x = _x;
	y = _y;
	z = _z;
	message = _message;
	orientation = _orientation;
	talkTime = _talkTime;
	
	//Show bubble pointing left
	backgroundImage.loadImage("bubble.png");
	
	//Show bubble point right
	backgroundImage_r.loadImage("bubble_2.png");
	
	font.loadFont("DIN.otf",12); //40 = Font size
	
	//Adjust this value to change the relative size of the bubble
	scale = .5;
	
	bubbleState = 0; // Default 0, no pos parameters set
	min = 0;
	sec = 0;
}

void talkBubble::update(){	
	
}

void talkBubble::draw(){
	
	ofPushMatrix();
	ofTranslate(0, 0, z);
		ofEnableAlphaBlending();
	
		if (active == 1) {
			ofSetColor(255,255,255,255);
		} else {
			ofSetColor(255,255,255,100);
		}
		
		if(orientation == 0)
		{
			backgroundImage.draw(x, y);
		} else if (orientation == 1) 
		{
			backgroundImage_r.draw(x, y);
		}
	
		
		ofSetColor(0,0,0,255);
		font.drawString(message,x + (backgroundImage.width/2)-50, y + (backgroundImage.height/2)-13);
	
		ofSetColor(255,255,255,255);
	
		//FORMAT TALK TIME
	
		if (talkTime > 59) {
			min = talkTime/60;
		}
		secsInMins = 60 * min;
		sec = talkTime - secsInMins;
		
		if(sec < 10)
		{
			cleanSeconds = "0" + ofToString(sec);
		} else {
			cleanSeconds = ofToString(sec);
		}

	font.drawString(ofToString(min) + ":" + cleanSeconds,x + (backgroundImage.width/2)-50, y + (backgroundImage.height/2)+20);
	ofDisableAlphaBlending();

	ofPopMatrix();
	ofSetColor(255,255,255,255);
}


void talkBubble::updateAttributes(int _active, string _name, int _talkTime)
{
	active = _active;
	message = _name;
	talkTime = _talkTime;
	
}

void talkBubble::updatePosition(float _x, float _y, int _z)
{
	x=_x;
	y=_y;
	z=_z;
	/*
	switch (bubbleState)
	{
		case 0:
			x = _x;
			y = _y;
			z = _z;	
			
			break;
		case 1:
			x = _x;
			y = _y;
			break;
		case 2:
			//
			break;
		default:
			//
			break;
	}
	*/
}

void talkBubble::setPosition()
{
	switch (bubbleState) 
	{
		case 0:
			bubbleState = 1;
		break;
		case 1:
			bubbleState = 2;
		break;
		case 2:
			orientation = 1;
			bubbleState = 3;
		break;
		case 3:
			orientation = 0;
			bubbleState = 0;
		break;

		default:
		//
		break;
	}
}