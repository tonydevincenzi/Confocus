//#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
//#include "ofxFBOTexture.h"
//#include "ofxShader.h"
//#include "shaderBlur.h"
#include "ofxOpenCv.h"

#include "grabVideo.h"

class testApp : public ofBaseApp{
	
	public:
		
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
		
		// TODO: move this when a separate "features" file is made.
		void highlightRGB(BYTE* videoBuff, USHORT* playerBuff, BYTE* highlightBuff, BYTE* overBuff);
		void adjustOver(int range, BYTE* overBuff);
		//bool	isInPlayerBound(int index, USHORT* playerBuff, int max_index);

		ofVideoGrabber grabber;
		bool hasCamera;

		//shaderBlur blur;
		KinectGrabber g_kinectGrabber;
		ofTexture		texColorAlpha;
		ofTexture		texGray;
		ofTexture		texHighlight;
		ofTexture		texOver;
		//ofxCvColorImage	texHighlight;
		//ofxCvColorImage	texHighlight2;
		
		unsigned char 	* colorAlphaPixels;
		unsigned char	* grayPixels;
		unsigned char	* highlightPixels;
		unsigned char	* overPixels;
		int headPositionX, headPositionY;
		
		//ofxFBOTexture texBlur;

};

#endif
	
