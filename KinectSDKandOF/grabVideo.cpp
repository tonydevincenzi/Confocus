//#include <fstream>
//using namespace std;

//#include "resource.h"
//#include <mmsystem.h>
//#include "stdafx.h"

//#include <tchar.h>
//#include <strsafe.h>
//#include <conio.h>



#include "stdafx.h"
#include "grabVideo.h"
#include "MSR_NuiApi.h"


void VideoGrabber::Video_Zero()
{
    //m_hNextDepthFrameEvent = NULL;
    m_hNextVideoFrameEvent = NULL;
    //m_hNextSkeletonEvent = NULL;
    //m_pDepthStreamHandle = NULL;
    m_pVideoStreamHandle = NULL;
    m_hThVideoProcess=NULL;
    m_hEvVideoProcessStop=NULL;
    //ZeroMemory(m_Pen,sizeof(m_Pen));
    //m_SkeletonDC = NULL;
    //m_SkeletonBMP = NULL;
    //m_SkeletonOldObj = NULL;
    //m_PensTotal = 6;
    //ZeroMemory(m_Points,sizeof(m_Points));
   // m_LastSkeletonFoundTime = -1;
    m_bScreenBlanked = false;
    m_FramesTotal = 0;
    //m_LastFPStime = -1;
    m_LastFramesTotal = 0;
}

HRESULT VideoGrabber::Video_Init()
{
	HRESULT hr;

    m_hNextVideoFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );    
	
	hr = NuiInitialize( 
        NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR);
    
	hr = NuiImageStreamOpen(
        NUI_IMAGE_TYPE_COLOR,
        NUI_IMAGE_RESOLUTION_640x480,
        0,
        2,
        m_hNextVideoFrameEvent,
        &m_pVideoStreamHandle );
    if( FAILED( hr ) )
    {
		printf("failed to open NuiImagesStream");
        return hr;
    }
	
	m_hEvVideoProcessStop=CreateEvent(NULL,FALSE,FALSE,NULL);
	//m_hThVideoProcess=CreateThread(NULL,0,Video_ProcessThread,this,0,NULL);

	return hr;
}


void VideoGrabber::Video_UnInit( )
{
    /*
    // Stop the Nui processing thread
    if(m_hEvVideoProcessStop!=NULL)
    {
        // Signal the thread
        SetEvent(m_hEvVideoProcessStop);

        // Wait for thread to stop
        if(m_hThVideoProcess!=NULL)
        {
            WaitForSingleObject(m_hThVideoProcess,INFINITE);
            CloseHandle(m_hThVideoProcess);
        }
        CloseHandle(m_hEvVideoProcessStop);
    }
	*/
    NuiShutdown( );

    if( m_hNextVideoFrameEvent && ( m_hNextVideoFrameEvent != INVALID_HANDLE_VALUE ) )
    {
        CloseHandle( m_hNextVideoFrameEvent );
        m_hNextVideoFrameEvent = NULL;
    }
	if( m_hEvVideoProcessStop && ( m_hEvVideoProcessStop != INVALID_HANDLE_VALUE ) )
    {
		CloseHandle(m_hEvVideoProcessStop);
	    m_hEvVideoProcessStop = NULL;
    }
	
}




//DWORD WINAPI VideoGrabber::Video_ProcessThread(LPVOID pParam)
int VideoGrabber::Video_Update()

{
    //VideoGrabber *pthis=(VideoGrabber *) pParam;
    HANDLE                hEvents[2];
    int                    nEventIdx;

    // Configure events to be listened on
    //hEvents[0]=pthis->m_hEvVideoProcessStop;
    //hEvents[1]=pthis->m_hNextVideoFrameEvent;
	hEvents[0]=m_hEvVideoProcessStop;
    hEvents[1]=m_hNextVideoFrameEvent;

    // Main thread loop
    //while(1)
    //{
        // Wait for an event to be signalled
        nEventIdx=WaitForMultipleObjects(sizeof(hEvents)/sizeof(hEvents[0]),hEvents,FALSE,100);
        //printf("index obtained %d\n",nEventIdx);
        // If the stop event, stop looping and exit
        if(nEventIdx==0)
        //    break;            
			return 1;

        // Process signal events
        switch(nEventIdx)
        {
            /*case 1:
                pthis->Nui_GotDepthAlert();
                pthis->m_FramesTotal++;
                break;
				*/
            //case 1:
                //pthis->Video_GotVideoAlert();
				//Video_GotVideoAlert();
                //break;
				/*
            case 3:
                pthis->Nui_GotSkeletonAlert( );
                break;*/
        }

		Video_GotVideoAlert();
		Video_GotSkeletonAlert();
    //}

    return (0);
}

void VideoGrabber::Video_GotVideoAlert( )
{
    const NUI_IMAGE_FRAME * pImageFrame = NULL;

    HRESULT hr = NuiImageStreamGetNextFrame(
        m_pVideoStreamHandle,
        0,
        &pImageFrame );
    if( FAILED( hr ) )
    {
		printf("Unable to get the frame after recieving alert for video frame \n");
        return;
    }

    NuiImageBuffer * pTexture = pImageFrame->pFrameTexture;
    KINECT_LOCKED_RECT LockedRect;
    pTexture->LockRect( 0, &LockedRect, NULL, 0 );
    if( LockedRect.Pitch != 0 )
    {
        //BYTE * pBuffer = (BYTE*) LockedRect.pBits;
		m_rgbBuffer = (BYTE*) LockedRect.pBits;
		//2560 bytes per line = 640 * 4 (4 bytes per pixel)
		//printf("%d\n", LockedRect.Pitch);
    }
    else
    {
        printf("buffer length of recieved texture is bogus\n");
    }
	NuiImageStreamReleaseFrame( m_pVideoStreamHandle, pImageFrame );
	Kinect_FormatRGBForOutput();
}

void VideoGrabber::Kinect_FormatRGBForOutput() {
	int totalPixels = KINECT_HEIGHT*KINECT_WIDTH*4;
	printf("%d\n",totalPixels);
	for (int i = 3; i < totalPixels; i= i + 4) {
		//set alpha to 255
		m_rgbBuffer[i] = 255;

		//invert the rgb order
		int blue = m_rgbBuffer[i-1];
		int green = m_rgbBuffer[i-2];
		int red = m_rgbBuffer[i-3];
		m_rgbBuffer[i-1] = red;
		m_rgbBuffer[i-3] = blue;

	}
}

void VideoGrabber::print_bytes( ) {
	//2560 bytes per line = 640 * 4 (4 bytes per pixel)
	//printf("byte data written: r:%d, g:%d, b:%d, other:%d\n", pBuffer[0], pBuffer[1], pBuffer[2], pBuffer[3]);
}

int VideoGrabber::getImageWidth() {
	return KINECT_WIDTH;
}

int VideoGrabber::getImageHeight() {
	return KINECT_HEIGHT;
}

BYTE* VideoGrabber::getAlphaPixels() {
	int totalPixels = 640*480*4;
	//printf("%d\n",totalPixels);
	for (int i = 3; i < totalPixels; i= i + 4) {
		m_rgbBuffer[i] = 255;
	}
	return m_rgbBuffer;
}

POINT         m_Points[NUI_SKELETON_POSITION_COUNT];
void VideoGrabber::Video_GotSkeletonAlert( )
{
    NUI_SKELETON_FRAME SkeletonFrame;

    HRESULT hr = NuiSkeletonGetNextFrame( 0, &SkeletonFrame );

    bool bFoundSkeleton = true;
	//POINT         m_Points[NUI_SKELETON_POSITION_COUNT];

    for( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )
    {
        if( SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED )
        {
            bFoundSkeleton = false;

			
			int scaleX = KINECT_WIDTH; //scaling up to image coordinates
			int scaleY = KINECT_HEIGHT;
			float fx=0,fy=0;
			//POINT         m_Points[NUI_SKELETON_POSITION_COUNT];
			NUI_SKELETON_DATA * pSkel;
			
			for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++)
			{
				pSkel=& SkeletonFrame.SkeletonData[i];
				NuiTransformSkeletonToDepthImageF( pSkel->SkeletonPositions[j], &fx, &fy );
				m_Points[j].x = (int) ( fx * scaleX + 0.5f );
				m_Points[j].y = (int) ( fy * scaleY + 0.5f );
			}

			if(m_Points[3].x!=0){
				//printf("headPosition");
				//printf("head Z=%4.2f /r", m_Points[3].x);
				//printf("%d\n",m_Points[3].x);
				//return m_Points[NUI_SKELETON_POSITION_COUNT];
            }else{
				NuiSkeletonGetNextFrame( 0, &SkeletonFrame );
            }

			/*
            float head_z=SkeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HEAD].z;
            if (head_z!=0){
            printf("head Z=%4.2f /r", head_z);
            }else{
            NuiSkeletonGetNextFrame( 0, &SkeletonFrame );
            }
			*/	
        }
    }

    // no skeletons!
    if( bFoundSkeleton )
    {
        return;
    }

    // smooth out the skeleton data
    NuiTransformSmooth(&SkeletonFrame,NULL);
}

void VideoGrabber::getJointsPoints() {
	headJoints_x=m_Points[3].x;
	headJoints_y=m_Points[3].y;
}

