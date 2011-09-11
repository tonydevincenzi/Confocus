#include "ofMain.h"
#include "talkBubble.h"

talkBubble::talkBubble(float _x, float _y, string _name, int _talkTime)
{
	x = _x;
	y = _y;
	name = _name;
	talkTime = _talkTime;
	
	//Show bubble pointing left
	backgroundImage.loadImage("images/bubble.png");
	
	//Show bubble point right
	backgroundImage_r.loadImage("images/bubble_2.png");
	
	font.loadFont("DIN.otf",12); //40 = Font size
	
	//Adjust this value to change the relative size of the bubble
	scale = .5;
	
	totalTime=0;
	//clickCount = 0;
	//name="default";
	active=false;
	
	bubbleState = 0; // Default 0, no pos parameters set
	min = 0;
	sec = 0;
	nameTx="";
}

void talkBubble::updateTimer(){	
	if(active)
	{
		timer();
	}
	else {
		startTime = 0;
	}
	
}

void talkBubble::draw(){
	
	//ofPushMatrix();
	//ofTranslate(0, 0, z);
	//	ofEnableAlphaBlending();
	
		if (active == 1) {
			ofSetColor(255,255,255,255);
		} else {
			ofSetColor(255,255,255,25);
		}

	    
		ofEnableAlphaBlending();
		backgroundImage.draw(x, y);
	
		
		ofSetColor(0,0,0,255);
		font.drawString(name,x + (backgroundImage.width/2)-50, y + (backgroundImage.height/2)-13);
		ofSetColor(255,255,255,255);
		
		//FORMAT TALK TIME	
		if (totalTime > 59) {
			min = totalTime/60;
		}
		secsInMins = 60 * min;
		sec = totalTime - secsInMins;
		
		if(sec < 10)
		{
			cleanSeconds = "0" + ofToString(sec);
		} else {
			cleanSeconds = ofToString(sec);
		}

		font.drawString(ofToString(min) + ":" + cleanSeconds,x + (backgroundImage.width/2)-50, y + (backgroundImage.height/2)+20);
		ofDisableAlphaBlending();

		//ofPopMatrix();
		//ofSetColor(255,255,255,255);		
}


void talkBubble::updateAttributes(string _name)
{
	//active = _active;
	name = _name;
	
}

void talkBubble::updatePosition(int _x, int _y)
{
	x=_x;
	y=_y;



    /*
	x=_x;
	y=_y;
	z=_z;
	
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

void talkBubble::timer(){
	if(startTime == 0)
		startTime=ofGetElapsedTimeMillis(); 
	
	deltaTime = ofGetElapsedTimeMillis() - startTime;
	
	if(deltaTime > 1000){
		totalTime++;
		deltaTime = 0;
		startTime = ofGetElapsedTimeMillis();
	}
}