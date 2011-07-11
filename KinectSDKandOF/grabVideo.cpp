//#include <fstream>
//using namespace std;

//#include "resource.h"
//#include <mmsystem.h>

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
        //MessageBoxResource(m_hWnd,IDS_ERROR_VIDEOSTREAM,MB_OK | MB_ICONHAND);
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
            case 1:
                //pthis->Video_GotVideoAlert();
				Video_GotVideoAlert();
                break;
				/*
            case 3:
                pthis->Nui_GotSkeletonAlert( );
                break;*/
        }
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
        BYTE * pBuffer = (BYTE*) LockedRect.pBits;
		//2560 bytes per line = 640 * 4 (4 bytes per pixel)
		printf("byte data written: r:%d, g:%d, b:%d, other:%d\n", pBuffer[0], pBuffer[1], pBuffer[2], pBuffer[3]);
    }
    else
    {
        printf("buffer length of recieved texture is bogus\n");
    }
	printf("frame end\n");
	NuiImageStreamReleaseFrame( m_pVideoStreamHandle, pImageFrame );
}


