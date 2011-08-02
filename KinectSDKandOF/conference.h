//#pragma once

#ifndef _CONF
#define _CONF

#include <windows.h>
#include "grabVideo.h"

const int DEPTH_THRESHOLD = 350;

void focusRGB(BYTE* videoBuff, USHORT* playerBuff, BYTE* focusBuff, BYTE* blurBuff, KinectGrabber* kinectGrabber);
void adjustOver(int range, BYTE* blurBuff);


enum applicationState {IDLE, SPEAKING}; 


/*
class conferenceManager {

	public:
		void focusRGB(BYTE* videoBuff, USHORT* playerBuff, BYTE* focusBuff, BYTE* blurBuff, KinectGrabber kinectGrabber);
		void adjustOver(int range, BYTE* blurBuff);



	private:
		enum applicationState {IDLE, SPEAKING}; 
		applicationState appState;


};*/

#endif