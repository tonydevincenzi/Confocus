//#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxFBOTexture.h"
#include "ofxShader.h"
#include "shaderBlur.h"
//#include "ofxOpenCv.h"

//#include "grabVideo.h"
#include "conference.h"
#include "button.h"
#include "slider.h"


//const int DEPTH_THRESHOLD = 350;
class testApp : public ofBaseApp{
	
	public:
		char eventString[255]; // for printing the location of the mouse (for debugging)

		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);


	private:
		KinectGrabber   g_kinectGrabber;
	
		ofTexture		texColorAlpha;
		ofTexture		texGray;
		ofTexture		texFocus;
		ofTexture		texBlur;
		//ofxCvColorImage	blurImg;		
		unsigned char 	* colorAlphaPixels;
		unsigned char	* grayPixels;
		unsigned char	* focusPixels;
		unsigned char	* blurPixels;

		int headPositionX, headPositionY, headPositionZ;
		int neckPositionX, neckPositionY, leftShoulderX, leftShoulderY, rightShoulderX, rightShoulderY,leftHandPX, leftHandPY, rightHandPX, rightHandPY;
		
		shaderBlur      blur;
		
		//GUI
		int             thresh;
		bool            activeFocus; //to deside the active or inActive focus
		
		//gui buttons
		button**        buttons;
		int             nButtons;
		bool            buttonPressed[6]; //the number of the boolean should equal to nButtons
		slider**        sliders;
		int             nSliders;
		ofImage         header;
		ofImage         header2;
		ofImage         bg;

};

#endif
	
