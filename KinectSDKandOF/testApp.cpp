#include "testApp.h"
#include <algorithm>
//#include "MSR_NuiApi.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofEnableAlphaBlending();
	
	ofBackground(255,255,255);	
	
	printf("initializing\n");
	g_kinectGrabber.Kinect_Zero();
	g_kinectGrabber.Kinect_Init();
	
	thresh = 300;
	focusPixels = new unsigned char [DEPTH_WIDTH*DEPTH_HEIGHT*4];
	blurPixels = new unsigned char [DEPTH_WIDTH*DEPTH_HEIGHT*4];

	texColorAlpha.allocate(VIDEO_WIDTH,VIDEO_HEIGHT,GL_RGBA);
	//texGray.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); // gray depth texture
	texFocus.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); 
	texBlur.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA);
	blurImg.allocate(DEPTH_WIDTH, DEPTH_HEIGHT);
	//shader.load("shaders/simpleBlurHorizontal.vert", "shaders/simpleBlurHorizontal.frag");
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	g_kinectGrabber.Kinect_Update();
	
	colorAlphaPixels = g_kinectGrabber.Kinect_getAlphaPixels();
	if(colorAlphaPixels != NULL) {
		texColorAlpha.loadData(colorAlphaPixels, VIDEO_WIDTH,VIDEO_HEIGHT, GL_RGBA);
	}
	/*
	grayPixels = (BYTE*)g_kinectGrabber.Kinect_getDepthPixels();	
	if (grayPixels != NULL) {
		texGray.loadData(grayPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	}*/
	
	USHORT* depthBuff = g_kinectGrabber.Kinect_getDepthBuffer();
	focusRGB(colorAlphaPixels, depthBuff, focusPixels, blurPixels);
	if(focusPixels != NULL) {
		//adjustOver(15, overPixels);
		texBlur.loadData(blurPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
		texFocus.loadData(focusPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	}
	blurImg.setFromPixels(focusPixels,DEPTH_WIDTH, DEPTH_HEIGHT);
	//int n= g_kinectGrabber.getJointsPixels();
	//printf("%d\n",n);
	
	g_kinectGrabber.getJointsPoints();
	headPositionX=g_kinectGrabber.headJoints_x;
	headPositionY=g_kinectGrabber.headJoints_y;
	headPositionZ=g_kinectGrabber.headJoints_z;
	//m_Points[3].z;
	//printf("position: %d\n",headPositionX);
	

}

//--------------------------------------------------------------
void testApp::draw(){

	
	//video image
	//shader.begin();
	//shader.setUniform(
	texColorAlpha.draw(0,0,VIDEO_WIDTH, VIDEO_HEIGHT);
	//shader.end();

	
	//diminished image

	blurImg.blurGaussian(25);
	blurImg.draw(640,0);
	texBlur.draw(640,0,VIDEO_WIDTH, VIDEO_HEIGHT);
	
	
	//texHighlight.draw(640,0,DEPTH_WIDTH,DEPTH_HEIGHT);
	//ofDisableAlphaBlending();
	//texOver.draw(640,0,DEPTH_WIDTH,DEPTH_HEIGHT);
	//texGray.draw(640,0,DEPTH_WIDTH,DEPTH_HEIGHT);

	ofSetHexColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, " threshold: %d (press: +/- to change) \n", thresh);
	ofDrawBitmapString(reportStr, 650, 10);


	ofCircle(headPositionX,headPositionY,20);
	

}
//-------------------------------------------------------------
void testApp::exit(){
	printf("cleaning up\n");
	g_kinectGrabber.Kinect_UnInit();
	free(focusPixels);
	free(blurPixels);
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	switch (key){
		case '+':
			thresh += 20;
			if (thresh > 1000) thresh = 1000;
			break;
		case '-':
			thresh -= 20;
			if (thresh < 10) thresh = 10;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//------------- -------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


//TODO: move this into a "features" file
void testApp::focusRGB(BYTE* videoBuff, USHORT* depthBuff, BYTE * focusBuff, BYTE* blurBuff) {
	  if (videoBuff && depthBuff) {
		LONG* pcolorx = new LONG();
		LONG* pcolory = new LONG();
		int max_index = DEPTH_WIDTH * DEPTH_HEIGHT * 4;

		for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
	
				g_kinectGrabber.Kinect_ColorFromDepth(x, y, pcolorx, pcolory);
				int index = (y * DEPTH_WIDTH) + x;
				int	color_index = ((*pcolory*VIDEO_WIDTH) + *pcolorx);
				
				focusBuff[3*index + 0] = videoBuff[4*color_index + 0];
				focusBuff[3*index + 1] = videoBuff[4*color_index + 1];
				focusBuff[3*index + 2] = videoBuff[4*color_index + 2];
				blurBuff[4*index + 0] = videoBuff[4*color_index + 0];
				blurBuff[4*index + 1] = videoBuff[4*color_index + 1];
				blurBuff[4*index + 2] = videoBuff[4*color_index + 2];
				//if that pixel does not belong to a player,  black it out (alpha = 255)
				//otherwise, display its rgb values
				if (depthBuff[index] > headPositionZ + thresh  || depthBuff[index] < headPositionZ - thresh ) {
					blurBuff[4*index + 3] = 0;
				} else {
					blurBuff[4*index + 3] = 255;
				}
			}
		}  
		free(pcolorx);
		free(pcolory);
	  }
}


/*
void testApp::highlightRGB(BYTE* videoBuff, USHORT* playerBuff, BYTE * highlightBuff, BYTE* overBuff) {
	  if (videoBuff && playerBuff) {
		LONG* pcolorx = new LONG();
		LONG* pcolory = new LONG();
		int max_index = DEPTH_WIDTH * DEPTH_HEIGHT * 4;

		for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
	
				g_kinectGrabber.Kinect_ColorFromDepth(x, y, pcolorx, pcolory);
				int index = (y * DEPTH_WIDTH) + x;
				int	color_index = ((*pcolory*VIDEO_WIDTH) + *pcolorx);
				//printf("depth (%d, %d)  color (%li, %li) \n", x, y, *pcolorx, *pcolory);
				//printf("index: %d color index: %d \n", index, color_index);
				
				//bool show = isInPlayerBound(index, playerBuff, max_index);

				highlightBuff[4*index + 0] = videoBuff[4*color_index + 0];
				highlightBuff[4*index + 1] = videoBuff[4*color_index + 1];
				highlightBuff[4*index + 2] = videoBuff[4*color_index + 2];
				highlightBuff[4*index + 3] = 255;
				overBuff[4*index + 0] = 0;
				overBuff[4*index + 1] = 0;
				overBuff[4*index + 2] = 0;
				//if that pixel does not belong to a player,  black it out
				//otherwise, leave its rgb values in tact
				if (playerBuff[index]) {
					overBuff[4*index + 3] = 0;
				} else {
					overBuff[4*index + 3] = 255;
				}
			}
		}  
		free(pcolorx);
		free(pcolory);
	  }
}
*/
/*
void testApp::adjustOver(int range, BYTE * overBuff) {
	// states:
	// 0 - have not seen a player pixel
	// 1 - scanning a player pixel
	// 2 - have not seen a player pixel, but no longer scanning a  player pixel
	int state = 0;

	for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
				
				int index = (y*DEPTH_WIDTH) + x;
	
				if (overBuff[4*index + 3] == 1) {
					
					int sum = 0;
					for (int i = -range; i <= range; i++ ) {
						sum += overBuff[(index - (DEPTH_WIDTH*i)) * 4 + 3];
					}
					int ave = sum/(2*range);
					overBuff[4*index + 3] =  min((int)overBuff[4*index + 3], ave);

				}
			}
	}
}

/*
void testApp::adjustOver(int range, BYTE * overBuff) {
	// states:
	// 0 - have not seen a player pixel
	// 1 - scanning a player pixel
	// 2 - have not seen a player pixel, but no longer scanning a  player pixel
	int state = 0;

	for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
				
				int index = (y*DEPTH_WIDTH) + x;
	
				if (overBuff[4*index + 3] == 0) {
					
					/*for (int i = 0; i <= range; i++ ) {
						//if (y + i < DEPTH_HEIGHT)
						//	overBuff[(index + (DEPTH_WIDTH*i)) * 4 + 3] =  min(i * (255 / range), (int)overBuff[(index+ (DEPTH_WIDTH*i)) * 4 + 3]); 

						if (y-i > 0)
							overBuff[(index - (DEPTH_WIDTH*i)) * 4 + 3] =  min(i * (255 / range), (int)overBuff[(index- (DEPTH_WIDTH*i)) * 4 + 3]);
					*/
					
/*					if (state == 0) {
						//do blending on left
						for (int i = 0; i < range; i++ ) {
							if (x - i >= 0)
								overBuff[(index - i) * 4 + 3] = min(i * (255 / range), (int)overBuff[(index- i) * 4 + 3]); 
						}
						state = 1;
					} else if (state == 1) {
						// the pixel is part of a person, make the overlay transparent
						overBuff[4*index + 3] = 0;
						state = 1;
					} else if (state == 2) {
						overBuff[4*index + 3] = 0;
						state = 1;
					}					
					
				} else if (overBuff[index*4 + 3] != 0) {
					
					if (state == 0) {
						// we haven't seen a person yet, keep the overlay
						state = 0;
					
					} else if (state == 1) {
						// blend on right
						for (int i = 0; i <= range; i++ ) {
							if (x + i <= DEPTH_WIDTH)
								overBuff[(index + i) * 4 + 3] = min(i * (255 / range), (int)overBuff[(index + i) * 4 + 3]); 
						}
						state = 2;

					} else if (state == 2) {
						if (x == DEPTH_WIDTH - 1) {
							state = 0;
						} else {
							state = 2;
						}
					}
				}
				
			
			}
	}

}*/
