#include "testApp.h"
#include <algorithm>

//--------------------------------------------------------------
void testApp::setup(){	

	//ofEnableAlphaBlending();	
	ofBackground(255,255,255);	            // Set the background color (right now, white)
	
	blur.setup(DEPTH_WIDTH, DEPTH_HEIGHT);  // set up the blur shader
	thresh=300;

	// initialize kinect and application
	g_kinectGrabber.Kinect_Zero();
	g_kinectGrabber.Kinect_Init();
	conference_init();
	
	// allocate memory for focus and blur pixels
	focusPixels = new unsigned char [DEPTH_WIDTH*DEPTH_HEIGHT*4];
	blurPixels = new unsigned char [DEPTH_WIDTH*DEPTH_HEIGHT*4];

	// allocate memory for textures
	texColorAlpha.allocate(VIDEO_WIDTH,VIDEO_HEIGHT,GL_RGBA);
	texFocus.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); 
	texBlur.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); 

	//texGray.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); // gray depth texture	

	//gui interface
	nButtons=6;
	buttons=new button*[nButtons];
	buttons[0]=new button("setup",40,ofGetHeight()-40,100,30,true);
	buttons[1]=new button("active",40+150,ofGetHeight()-40,100,30,true);
	buttons[2]=new button("manual",40+150+150,ofGetHeight()-40,100,30,true);
	buttons[3]=new button("focus",40,60+ofGetHeight()-270,100,30,false);
	buttons[4]=new button("black",40,100+ofGetHeight()-270,100,30,false);
	buttons[5]=new button("zoom", 40,140+ofGetHeight()-270,100,30,false);
	for(int i=0;i<nButtons;i++) buttonPressed[i]=false;
	buttonPressed[1]=true;
	buttonPressed[0]=true;
	buttonPressed[3]=true;

	nSliders=3;
	sliders=new slider*[nSliders];
	sliders[0]=new slider(200,60-20+ofGetHeight()-270,200);
	sliders[1]=new slider(200,100-20+ofGetHeight()-270,200);	
	sliders[2]=new slider(200,140-20+ofGetHeight()-270,200);
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	g_kinectGrabber.Kinect_Update();
	//conference_update();
    
	// load the RGBA values into a texture
	colorAlphaPixels = g_kinectGrabber.Kinect_getAlphaPixels();
	if(colorAlphaPixels != NULL) {
		texColorAlpha.loadData(colorAlphaPixels, VIDEO_WIDTH,VIDEO_HEIGHT, GL_RGBA);
	}

	/*
	grayPixels = (BYTE*)g_kinectGrabber.Kinect_getDepthPixels();	
	if (grayPixels != NULL) {
		texGray.loadData(grayPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	}*/
	
	// load the RGBA values into the blur and focus textures for the diminshed reality image
	USHORT* depthBuff = g_kinectGrabber.Kinect_getDepthBuffer();
	focusRGB(colorAlphaPixels, depthBuff, focusPixels, blurPixels, &g_kinectGrabber);	
	/*
	if(focusPixels != NULL) {
		
		adjustOver(2, blurPixels);
		texBlur.loadData(blurPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	}
	*/
	texFocus.loadData(focusPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	texBlur.loadData(blurPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);

    // Gets the joint positions
	g_kinectGrabber.getJointsPoints();
	headPositionX=g_kinectGrabber.headJoints_x;
	headPositionY=g_kinectGrabber.headJoints_y;
	headPositionZ=g_kinectGrabber.headJoints_z;

	
	// TODO: move this somewhere else. Probably goes in the conference.cpp file?
	// Find the skeleton index of the individuals head position is closest to that of the audio position.
	double minSoundDiscrepancy = VIDEO_WIDTH;
	printf("-------------------------------------------\n"); 
	printf(" Head Positions \n"); 
	printf("-------------------------------------------\n"); 
	// Loop through all of the 
	for (int i = 0; i < 6; i ++) { 
	// TODO: value should be some constant indicating number of skeletons
		
		// print out the x values for each of the players head tracking
		printf("head %d x value: %d \n", i, g_kinectGrabber.headXValues[i]);

		// compare to find the reading with the smallest discrepancy from the sound localization
		double discrepancy = abs(g_kinectGrabber.headXValues[i] - g_kinectGrabber.soundPixel);
		if (discrepancy < minSoundDiscrepancy) {
			minSoundDiscrepancy = discrepancy;
			g_kinectGrabber.minDiscrepancyIdx = i;
		}
	}
	// print the closest match
	printf(" closest person : %i \n", g_kinectGrabber.minDiscrepancyIdx); 
	printf("-------------------------------------------\n"); 
}

//--------------------------------------------------------------
void testApp::draw(){
	//video image
	texColorAlpha.draw(640,0,VIDEO_WIDTH, VIDEO_HEIGHT);
	ofEnableAlphaBlending();
	
	//diminished image
	texFocus.draw(0,0,DEPTH_WIDTH, DEPTH_HEIGHT); //draw the focus texture	
	blur.setBlurParams(4,(float)100/100);
	blur.beginRender();
	texBlur.draw(0,0,DEPTH_WIDTH, DEPTH_HEIGHT); //always 0
	blur.endRender();
	blur.draw(0, 0, 640, 480, true);
	ofDisableAlphaBlending();
	//texGray.draw(640,0,DEPTH_WIDTH,DEPTH_HEIGHT);

	//circle drawn on head of tracked individual
	ofCircle(headPositionX+640,headPositionY,10);
	ofCircle(g_kinectGrabber.shoulderLeft_x+640, g_kinectGrabber.shoulderLeft_y,10);
	ofCircle(g_kinectGrabber.shoulderRight_x+640,g_kinectGrabber.shoulderRight_y,10); 
	ofCircle(g_kinectGrabber.handLeft_x+640,g_kinectGrabber.handLeft_y,10); 
	ofCircle(g_kinectGrabber.handRight_x+640,g_kinectGrabber.handRight_y,10);
	ofLine(headPositionX+640,headPositionY,g_kinectGrabber.shoulderLeft_x+640, g_kinectGrabber.shoulderLeft_y);
	ofLine(g_kinectGrabber.shoulderLeft_x+640, g_kinectGrabber.shoulderLeft_y,g_kinectGrabber.handLeft_x+640,g_kinectGrabber.handLeft_y);
	ofLine(headPositionX+640,headPositionY,g_kinectGrabber.shoulderRight_x+640,g_kinectGrabber.shoulderRight_y);
	ofLine(g_kinectGrabber.shoulderRight_x+640,g_kinectGrabber.shoulderRight_y,g_kinectGrabber.handRight_x+640,g_kinectGrabber.handRight_y);

	// for debugging, draws the current mouse position
	//ofSetColor(0x00000);
	//ofDrawBitmapString(eventString, 650, 500);
	//ofSetColor(0xffffff);

	//ofPushMatrix();
	// gui interface
	for(int i=0;i<3;i++) buttons[i]->drawFont(buttonPressed[i]);   //draw 3 buttons always existing at the bottom
	if(buttonPressed[0]){  //draw 3 buttons triggered by pressing the setUp button; boolean trigger is used to disable the button pressing if it's not shown on the screen
		for(int i=3;i<nButtons;i++){
			buttons[i]->trigger=true;
			buttons[i]->drawFont(buttonPressed[i]);
		}
		for(int i=0;i<nSliders;i++) sliders[i]->drawSlider(0,100);
	} else if(!buttonPressed[0]){
		for(int i=3;i<nButtons;i++) buttons[i]->trigger=false;
	}
	//ofPopMatrix();
	ofSetColor(0xffffff);
	
}
//-------------------------------------------------------------
void testApp::exit(){
	//printf("cleaning up\n");
	g_kinectGrabber.Kinect_UnInit();
	free(focusPixels);
	free(blurPixels);
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	/*switch (key){
		case '+':
			thresh += 20;
			if (thresh > 1000) thresh = 1000;
			break;
		case '-':
			thresh -= 20;
			if (thresh < 10) thresh = 10;
			break;
	}*/
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//------------- -------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	sprintf(eventString, "mouseMoved = (%i,%i)", x, y);
		
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	for(int i=0;i<nSliders;i++) sliders[i]->getSliderPosX(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(buttons[0]->buttonPressed(x,y)) buttonPressed[0]=!buttonPressed[0]; //setUpbutton
	if(buttons[1]->buttonPressed(x,y)){
		buttonPressed[1]=true;
		buttonPressed[2]=false;
	}
	if(buttons[2]->buttonPressed(x,y)){
		buttonPressed[2]=true;
		buttonPressed[1]=false;
	}
	if(buttons[3]->buttonPressed(x,y)){
		buttonPressed[3]=true;
		buttonPressed[4]=false;
		buttonPressed[5]=false;
	}
	if(buttons[4]->buttonPressed(x,y)){
		buttonPressed[4]=true;
		buttonPressed[3]=false;
		buttonPressed[5]=false;
	}	
	if(buttons[5]->buttonPressed(x,y)){
		buttonPressed[5]=true;
		buttonPressed[3]=false;
		buttonPressed[4]=false;
		printf("buttonPressed \n");
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
