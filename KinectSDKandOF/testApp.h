#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxFBOTexture.h"
#include "ofxShader.h"

#include "shaderBlur.h"
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
		
		ofVideoGrabber grabber;
		bool hasCamera;

		shaderBlur blur;
		VideoGrabber g_videoGrabber;
		ofTexture		texColorAlpha;
		unsigned char 	* colorAlphaPixels;
		ofTexture		texGray;
		unsigned char 	* grayPixels;
		
		int imageWidth; 
		int imageHeight; 
};

#endif
	
