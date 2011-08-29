#include "testApp.h"
#include <algorithm>

//--------------------------------------------------------------
void testApp::setup(){	

	//ofEnableAlphaBlending();	
	ofBackground(0,0,0);	            // Set the background color (right now, white)
	
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

	//other parameters
	maskValue=3;
	  
	//gui interface
	nButtons=9;
	buttons=new button*[nButtons];
	buttons[0]=new button("setup",55,759,100,30,true,"images/set_a.png","images/set_b.png");
	buttons[1]=new button("active",234,759,100,30,true,"images/auto_a.png","images/auto_b.png");
	buttons[2]=new button("manual",326,759,100,30,true,"images/manual_a.png","images/manual_b.png");
	
	buttons[3]=new button("focus",29,529,100,30,false,"images/focus_a.png","images/focus_b.png");
	buttons[4]=new button("black",29,599,100,30,false,"images/mask_a.png","images/mask_b.png");
	buttons[5]=new button("zoom", 29,667,100,30,false,"images/zoom_a.png","images/zoom_b.png");
	
	buttons[6]=new button("sketchViewer", 522,759,100,30,true,"images/sketch_a.png","images/sketch_b.png");
	buttons[7]=new button("bubble", 107,759,100,30,true,"images/bubble_a.png","images/bubble_b.png");
	buttons[8]=new button("ipad", 570,759,100,30,true,"images/ipad_a.png","images/ipad_b.png");

	for(int i=0;i<nButtons;i++) buttonPressed[i]=false;
	buttonPressed[1]=true;
	buttonPressed[0]=true;
	buttonPressed[3]=true;
	//buttonPressed[6]=true;

	nSliders=3;
	sliders=new slider*[nSliders];
	sliders[0]=new slider(137,540,442);
	sliders[1]=new slider(137,609,442);	
	sliders[2]=new slider(137,678,442);

	header.loadImage("images/head.png");
	header2.loadImage("images/raw.png");
	bg.loadImage("images/bg.png");

	
	//talk bubbles
	nBubbles = 6; 
	talkBubbles = new talkBubble*[nBubbles];   
	talkBubbles[0] = new talkBubble(0,0,"name 0", 100);
	talkBubbles[1] = new talkBubble(0,0,"name 1", 100);
	talkBubbles[2] = new talkBubble(0,0,"name 2", 100);
	talkBubbles[3] = new talkBubble(0,0,"name 3", 100);	
	talkBubbles[4] = new talkBubble(0,0,"name 4", 100);	
	talkBubbles[5] = new talkBubble(0,0,"name 5", 100);

	//sketch viewer
	sketchShareView.initViewer();	
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(0,0,0);
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
	if(buttonPressed[1]) focusRGB(colorAlphaPixels, depthBuff, focusPixels, blurPixels, &g_kinectGrabber,buttonPressed[3],buttonPressed[4],buttonPressed[5],maskValue);	
	if(buttonPressed[2]) focusRGB_manual(colorAlphaPixels, depthBuff, focusPixels, blurPixels, &g_kinectGrabber,buttonPressed[3],buttonPressed[4],buttonPressed[5],mouseX,mouseY);	
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

			talkBubbles[i]->active=true; //the active talk bubbles
			for(int j=i+1;j<6;j++) talkBubbles[j]->active=false; //de-active other bubbles
			for(int j=i-1;j>=0;j--) talkBubbles[j]->active=false;
		}

		//talk bubbles update
		int headPositionX = g_kinectGrabber.headXValues[i];
		int headPositionY = g_kinectGrabber.headYValues[i];
		talkBubbles[i]->updatePosition(headPositionX,headPositionY);
		talkBubbles[i]->updateTimer();
	}
	// print the closest match
	printf(" closest person : %i \n", g_kinectGrabber.minDiscrepancyIdx); 
	printf("-------------------------------------------\n"); 

	//sketch viewer
	sketchShareView.update();
	if(buttonPressed[6]) sketchShareView.close=false;
	else if(!buttonPressed[6]) sketchShareView.close=true;

}

//--------------------------------------------------------------
void testApp::draw(){
	ofEnableSmoothing();

	int blurParam; //different mode has different blurParameter control
	float scaleParam;	
	//for(int i=0;i<3;i++){
	//	if(buttonPressed[i+3]) blurParam=sliders[i]->value;
	//}

	if(buttonPressed[3]) {
		blurParam=sliders[0]->value;
		scaleParam=1;
	}
	if(buttonPressed[4]) {
		blurParam=60;
		scaleParam=1;
		maskValue=sliders[1]->value;
	}
	if(buttonPressed[5]) {
		blurParam=60;
		scaleParam=sliders[2]->value;
	}

	//diminished image
	ofEnableAlphaBlending();
	texFocus.draw(0,0+25,DEPTH_WIDTH*scaleParam, DEPTH_HEIGHT*scaleParam); //draw the focus texture	
	
	blur.setBlurParams(4,(float)blurParam/100);
	blur.beginRender();
	texBlur.draw(0,0,DEPTH_WIDTH, DEPTH_HEIGHT); //always 0
	blur.endRender();
	blur.draw(0, 0+25, DEPTH_WIDTH*scaleParam, DEPTH_HEIGHT*scaleParam, true);
	ofDisableAlphaBlending();
	//texGray.draw(640,0,DEPTH_WIDTH,DEPTH_HEIGHT);

	ofSetColor(0x000000);
	ofRect(640,0+25,VIDEO_WIDTH,ofGetHeight());
	ofSetColor(0xffffff);

	//video image
	texColorAlpha.draw(640+20,0+25,VIDEO_WIDTH, VIDEO_HEIGHT);

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

	// gui interface
	header.draw(0,0);
	header2.draw(640+20,0);
	bg.draw(1,500);

	ofEnableAlphaBlending();
	for(int i=0;i<3;i++) buttons[i]->drawFont(buttonPressed[i]);   //draw 3 buttons always existing at the bottom
	buttons[6]->drawFont(buttonPressed[6]); //draw 7th button always existing
	buttons[7]->drawFont(buttonPressed[7]);
	buttons[8]->drawFont(buttonPressed[8]);
	if(buttonPressed[0]){  //draw 3 buttons triggered by pressing the setUp button; boolean trigger is used to disable the button pressing if it's not shown on the screen
		for(int i=3;i<6;i++){
			buttons[i]->trigger=true;
			buttons[i]->drawFont(buttonPressed[i]);
		}
		//for(int i=0;i<nSliders;i++) sliders[i]->drawSlider(80,400);
		sliders[0]->drawSlider(80,400);
		sliders[1]->drawSlider(4,0.01);
		sliders[2]->drawSlider(0.8,1.5);
	} else if(!buttonPressed[0]){
		for(int i=3;i<6;i++) buttons[i]->trigger=false;
	}
	ofDisableAlphaBlending();
	//ofSetColor(0xffffff);

	//talk bubble
	for(int i=0;i<nBubbles;i++) talkBubbles[i]->draw();

	//sketch viewer
	ofEnableAlphaBlending();
	if(!sketchShareView.close){
		sketchShareView.drawBg();
		sketchShareView.drawVideo();
	}
	ofDisableAlphaBlending();

	
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
	
	for (int i=0;i<nBubbles;i++){
		if (talkBubbles[i]->active) {
			if(key == '-') talkBubbles[i]->name.erase();
			else  talkBubbles[i]->name.append(1,(char)key);
		}
	}
	
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
	//button pressing
	if(buttons[0]->buttonPressed(x,y)) buttonPressed[0]=!buttonPressed[0]; //setUpbutton
	if(buttons[6]->buttonPressed(x,y)) buttonPressed[6]=!buttonPressed[6]; //turn on/off the sketchviewer
	
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
	//if(buttons[6]->buttonPressed(x,y)) buttonPressed[6]=!buttonPressed[6];  //turn on and off the sketchviewer

	//sketchViewer
	//sketchShareView.closeDetect(x,y);
	sketchShareView.zoomDetect(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
