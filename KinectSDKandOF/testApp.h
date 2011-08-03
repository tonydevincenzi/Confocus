//#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxFBOTexture.h"
#include "ofxShader.h"
#include "shaderBlur.h"
#include "ofxOpenCv.h"
//#include "ofShader.h"

//#include "grabVideo.h"
#include "conference.h"


//const int DEPTH_THRESHOLD = 350;
class testApp : public ofBaseApp{
	
	public:
		

		char eventString[255];


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
		
		//TODO: move this
		//void focusRGB(BYTE* videoBuff, USHORT* playerBuff, BYTE* focusBuff, BYTE* blurBuff);
		//void adjustOver(int range, BYTE* blurBuff);

	private:
	
		//conferenceManager conference;
		ofVideoGrabber grabber;
		bool hasCamera;

		KinectGrabber g_kinectGrabber;
		
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
		
		shaderBlur      blur;
		int             thresh;
		//ofxFBOTexture texBlur;
		//ofShader shader;

};

#endif
	
