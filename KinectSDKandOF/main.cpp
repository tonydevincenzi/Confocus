
/*
// For configuring DMO properties
#include <wmcodecdsp.h>

// For discovering microphone array device
#include <MMDeviceApi.h>
#include <devicetopology.h>

// For functions and definitions used to create output file
#include <dmo.h> // Mo*MediaType
#include <uuids.h> // FORMAT_WaveFormatEx and such
#include <mfapi.h> // FCC
*/
#include <windows.h>

// For string input,output and manipulation
#include <tchar.h>
#include <strsafe.h>
#include <conio.h>
#include <time.h>

// For CLSID_CMSRKinectAudio GUID
//#include "MSRKinectAudio.h"
#include "grabVideo.h"

#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

VideoGrabber	g_videoGrabber;

//========================================================================

void wait ( int seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}

int main( ){

	printf("initializing\n");
	g_videoGrabber.Video_Zero();
	g_videoGrabber.Video_Init();
	printf("gathering data\n");
//	wait(10);

	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

	printf("cleaning up\n");
	g_videoGrabber.Video_UnInit();

}




