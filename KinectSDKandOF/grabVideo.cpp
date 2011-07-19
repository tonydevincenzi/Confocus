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


void KinectGrabber::Kinect_getPixPos(LONG depthX, LONG depthY, USHORT depthValue, LONG *pColorX, LONG *pColorY) {
	NuiImageGetColorPixelCoordinatesFromDepthPixel( 
		NUI_IMAGE_RESOLUTION_640x480,//_In_ NUI_IMAGE_RESOLUTION eColorResolution,
		NULL, //_In_opt_ CONST NUI_IMAGE_VIEW_AREA *pcViewArea,
		LONG(depthX), LONG(depthY), depthValue , pColorX, pColorY); 
}

void KinectGrabber::Kinect_Zero()
{
    m_hNextDepthFrameEvent = NULL;
    m_hNextVideoFrameEvent = NULL;
    m_hNextSkeletonEvent = NULL;
    m_pDepthStreamHandle = NULL;
    m_pVideoStreamHandle = NULL;
   // m_hThVideoProcess=NULL;
   // m_hEvVideoProcessStop=NULL;
    //ZeroMemory(m_Pen,sizeof(m_Pen));
    //m_SkeletonDC = NULL;
    //m_SkeletonBMP = NULL;
    //m_SkeletonOldObj = NULL;
    //m_PensTotal = 6;
    //ZeroMemory(m_Points,sizeof(m_Points));
    //m_LastSkeletonFoundTime = -1;
    m_bScreenBlanked = false;
    m_FramesTotal = 0;
    //m_LastFPStime = -1;
    m_LastFramesTotal = 0;
	//m_rgbBuffer = NULL;
	newRGBData = false;
}

HRESULT KinectGrabber::Kinect_Init() {
	
	HRESULT hr;

    m_hNextVideoFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );    
	m_hNextDepthFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );    
	m_hNextSkeletonFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );    
	
	hr = NuiInitialize( 
        NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR);
    if( FAILED( hr ) )
    {
		printf("failed to inialize nui");
	}
	hr = NuiSkeletonTrackingEnable( m_hNextSkeletonEvent, 0 );
    if( FAILED( hr ) )
    {
		printf("failed to open skeleton tracking.");//    MessageBoxResource(m_hWnd,IDS_ERROR_SKELETONTRACKING,MB_OK | MB_ICONHAND);
        return hr;
    }
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
	hr = NuiImageStreamOpen(
        NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,
        NUI_IMAGE_RESOLUTION_320x240,
        0,
        2,
        m_hNextDepthFrameEvent,
        &m_pDepthStreamHandle );
    if( FAILED( hr ) )
    {
    	printf("failed to open NuiImagesStream");
        return hr;
    }

	return hr;
}


void KinectGrabber::Kinect_UnInit( )
{

    NuiShutdown( );

    if( m_hNextVideoFrameEvent && ( m_hNextVideoFrameEvent != INVALID_HANDLE_VALUE ) )
    {
        CloseHandle( m_hNextVideoFrameEvent );
        m_hNextVideoFrameEvent = NULL;
    }
    if( m_hNextDepthFrameEvent && ( m_hNextDepthFrameEvent != INVALID_HANDLE_VALUE ) )
    {
        CloseHandle( m_hNextDepthFrameEvent );
        m_hNextDepthFrameEvent = NULL;
    }
    if( m_hNextSkeletonFrameEvent && ( m_hNextSkeletonFrameEvent != INVALID_HANDLE_VALUE ) )
    {
        CloseHandle( m_hNextSkeletonFrameEvent );
        m_hNextSkeletonFrameEvent = NULL;
    }
}




//DWORD WINAPI KinectGrabber::Video_ProcessThread(LPVOID pParam)
int KinectGrabber::Kinect_Update()

{
    //KinectGrabber *pthis=(KinectGrabber *) pParam;
    HANDLE                hEvents[3];
    int                    nEventIdx;

    // Configure events to be listened on
    //hEvents[0]=m_hEvVideoProcessStop;
    hEvents[0]=m_hNextVideoFrameEvent;
	hEvents[1]=m_hNextDepthFrameEvent;
	hEvents[2]=m_hNextSkeletonFrameEvent;
    
    // Wait for an event to be signalled
    nEventIdx=WaitForMultipleObjects(sizeof(hEvents)/sizeof(hEvents[0]),hEvents,FALSE,INFINITE);
    if (nEventIdx ==2) 
	printf("index obtained %d\n",nEventIdx);
    // If the stop event, stop looping and exit
    //if(nEventIdx==0)
        //break;            
	//	return 1;
		
        // Process signal events
        switch(nEventIdx)
        {
            case 0:
				newRGBData = true;
                Kinect_GotVideoAlert();
				break;
			
			case 1:
                Kinect_GotDepthAlert();
                break;
					
            case 2:
          //      Kinect_GotSkeletonAlert();
                break;
        }
		Kinect_GotSkeletonAlert();
		

    return (0);
}

void KinectGrabber::Kinect_GotVideoAlert( )
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
		//EnterCriticalSection(&rgbLock);
		//printf("grabbing pixel data\n");	
        memcpy(m_rgbBuffer, LockedRect.pBits, sizeof(BYTE) * VIDEO_HEIGHT * VIDEO_WIDTH * 4);
		//m_rgbBuffer = (BYTE*) LockedRect.pBits;
		Kinect_FormatRGBForOutput();
			
		//2560 bytes per line = 640 * 4 (4 bytes per pixel)
		//LeaveCriticalSection(&rgbLock);
		//printf("pixel data ready\n");	
	}
    else
    {
        printf("buffer length of recieved texture is bogus\n");
    }
	NuiImageStreamReleaseFrame( m_pVideoStreamHandle, pImageFrame );
}


void KinectGrabber::Kinect_FormatRGBForOutput() {
	int totalPixels = VIDEO_HEIGHT*VIDEO_WIDTH*4;
	//printf("%d\n",totalPixels);
	for (int i = 3; i < totalPixels; i= i + 4) {
		//set alpha to 255
		m_rgbBuffer[i] = 255;

		//invert the rgb order
		int blue = m_rgbBuffer[i-1];
		//int green = m_rgbBuffer[i-2];
		int red = m_rgbBuffer[i-3];
		m_rgbBuffer[i-1] = red;
		m_rgbBuffer[i-3] = blue;

	}
	for (int i = 3; i < totalPixels; i= i + 4) {
		m_rgbBuffer[i] = 255;
	}
}

void KinectGrabber::Kinect_GotDepthAlert( ) {
	const NUI_IMAGE_FRAME * pImageFrame = NULL;

    HRESULT hr = NuiImageStreamGetNextFrame(
        m_pDepthStreamHandle,
        0,
        &pImageFrame );

    if( FAILED( hr ) )
    {
    	printf("Unable to get the frame after recieving alert for depth frame \n");
		return;
    }

    NuiImageBuffer * pTexture = pImageFrame->pFrameTexture;
    KINECT_LOCKED_RECT LockedRect;
    pTexture->LockRect( 0, &LockedRect, NULL, 0 );
    if( LockedRect.Pitch != 0 )
    {
        BYTE * pBuffer = (BYTE*) LockedRect.pBits;

        // draw the bits to the bitmap
        RGBQUAD * rgbrun = m_rgbDepth;
		USHORT * depthrun = m_depthBuffer;
		USHORT * playerrun = m_playerBuffer;
        USHORT * pBufferRun = (USHORT*) pBuffer;
        for( int y = 0 ; y < DEPTH_HEIGHT ; y++ )
        {
            for( int x = 0 ; x < DEPTH_WIDTH ; x++ )
            {
				// set the color (just for visualization)
                RGBQUAD quad = Kinect_DepthToRGB( *pBufferRun );
                *rgbrun = quad;
                rgbrun++;
				
				USHORT RealDepth = (*pBufferRun & 0xfff8) >> 3;
				*depthrun = RealDepth;
				depthrun++;

				USHORT Player = *pBufferRun  & 7;
				*playerrun = Player;
				playerrun++;

				//inc buffer pointer
				pBufferRun++;
            }
        }
		
    }
    else
    {
        printf( "Buffer length of received texture is bogus\r\n" );
    }

    NuiImageStreamReleaseFrame( m_pDepthStreamHandle, pImageFrame );

}

void KinectGrabber::print_bytes( ) {
	//2560 bytes per line = 640 * 4 (4 bytes per pixel)
	//printf("byte data written: r:%d, g:%d, b:%d, other:%d\n", pBuffer[0], pBuffer[1], pBuffer[2], pBuffer[3]);
}


BYTE* KinectGrabber::Kinect_getAlphaPixels() {
	if (newRGBData) {
		newRGBData = false;
		return m_rgbBuffer;
	}
	return NULL;
}

/*CRITICAL_SECTION* KinectGrabber::Kinect_getRGBLock() {
	return &rgbLock;
}*/


USHORT* KinectGrabber::Kinect_getPlayerBuffer() {
	return m_playerBuffer;
}

USHORT* KinectGrabber::Kinect_getDepthBuffer() {
	return m_depthBuffer;
}

//use for visualizing depth
RGBQUAD* KinectGrabber::Kinect_getDepthPixels() {
	return m_rgbDepth;
}
RGBQUAD KinectGrabber::Kinect_DepthToRGB( USHORT s )
{
    USHORT RealDepth = (s & 0xfff8) >> 3;

    // transform 13-bit depth information into an 8-bit intensity appropriate
    // for display (we disregard information in most significant bit)
    BYTE l = 255 - (BYTE)(256*RealDepth/0x0fff);

    RGBQUAD q;
    
    q.rgbRed = l / 2;
    q.rgbBlue = l / 2;
    q.rgbGreen = l / 2;
 
	return q;
}


POINT         m_Points[NUI_SKELETON_POSITION_COUNT];
void KinectGrabber::Kinect_GotSkeletonAlert( )
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

			
			int scaleX = VIDEO_WIDTH; //scaling up to image coordinates
			int scaleY = VIDEO_HEIGHT;
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
			/*
			if(m_Points[3].x!=0){
				//printf("headPosition");
				//printf("head Z=%4.2f /r", m_Points[3].x);
				//printf("%d\n",m_Points[3].x);
				//return m_Points[NUI_SKELETON_POSITION_COUNT];
            }else{
				NuiSkeletonGetNextFrame( 0, &SkeletonFrame );
            }
			*/
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

void KinectGrabber::getJointsPoints() {
	headJoints_x=m_Points[3].x;
	headJoints_y=m_Points[3].y;
}
