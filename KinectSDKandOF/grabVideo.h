#pragma once
//#ifndef GRAB_VIDEO_H
//#define GRAB_VIDEO_H




#include <iostream>
#include <vector>
//#include <process.h>

#include <tchar.h>
#include <strsafe.h>
#include <conio.h>

// For functions and definitions used to create output file
#include <dmo.h> // Mo*MediaType
#include <uuids.h> // FORMAT_WaveFormatEx and such
#include <mfapi.h> // FCC

#include "resource.h"


const int VIDEO_WIDTH = 640;
const int VIDEO_HEIGHT= 480;
const int DEPTH_WIDTH = 640;
const int DEPTH_HEIGHT= 480;


class KinectGrabber
{
public:
    HRESULT					Kinect_Init();
    void                    Kinect_UnInit( );
    void                    Kinect_GotVideoAlert( );
	void                    Kinect_GotDepthAlert( );
    void                    Kinect_GotSkeletonAlert( );
    void                    Kinect_Zero();
	int						Kinect_Update();
	void					print_bytes();
	RGBQUAD*		Kinect_getDepthPixels();
	BYTE*			Kinect_getAlphaPixels();
	BYTE*			Kinect_getRGBBuffer();
	USHORT*			Kinect_getDepthBuffer();
	USHORT*			Kinect_getPlayerBuffer();
	void            getJointsPoints();
	void			Kinect_ColorFromDepth(LONG depthX, LONG depthY, LONG *pColorX, LONG *pColorY);
	void			Kinect_makeRGBFromRGBA();
	HRESULT			DShowRecord(IMediaObject* pDMO, IPropertyStore* pPS);

	//void                    Nui_BlankSkeletonScreen( HWND hWnd );
    //void                    Nui_DoDoubleBuffer(HWND hWnd,HDC hDC);
    //void                    Nui_DrawSkeleton( bool bBlank, NUI_SKELETON_DATA * pSkel);
    //void                    Nui_DrawSkeletonSegment( NUI_SKELETON_DATA * pSkel, int numJoints, ... );

    //RGBQUAD                 Nui_ShortToQuad_Depth( USHORT s );

    //static LONG CALLBACK    WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    //HWND m_hWnd;
	int minDiscrepancyIdx;
	double soundPixel;

	int           headJoints_x;
	int           headJoints_y;
	int    headJoints_z;
	int           handLeft_x;
	int           handLeft_y;
	int           handRight_x;
	int           handRight_y;
	int           shoulderLeft_x;
	int           shoulderLeft_y;
	int           shoulderRight_x;
	int           shoulderRight_y;


private:
    
	// Audio stuff
	HANDLE			 mmHandle;
	IMediaObject*	pDMO;  
    IPropertyStore*	pPS;
    double dConf;
	double dBeamAngle, dAngle;	
	double			soundAngle;

	// Nui stuff
	void		  Kinect_FormatRGBForOutput();
	RGBQUAD       Kinect_DepthToRGB( USHORT s );
    HANDLE        m_hNextSkeletonFrameEvent;
    HANDLE        m_hNextDepthFrameEvent;
    HANDLE        m_hNextVideoFrameEvent;
    HANDLE        m_hNextDepthPlayerFrameEvent;
	HANDLE        m_pDepthStreamHandle;
	HANDLE        m_pDepthPlayerStreamHandle;
    HANDLE        m_pVideoStreamHandle;
    //HFONT         m_hFontFPS;
    //HPEN          m_Pen[6];
    //HDC           m_SkeletonDC;
    //HBITMAP       m_SkeletonBMP;
    //HGDIOBJ       m_SkeletonOldObj;
    //int           m_PensTotal;
    //POINT         m_Points[NUI_SKELETON_POSITION_COUNT];
    //RGBQUAD       m_rgbWk[640*480];
    //int           m_LastSkeletonFoundTime;
    //bool          m_bScreenBlanked;
    //int           m_FramesTotal;
    //int           m_LastFPStime;
	int				headXValues[8];


	// Video stuff
    int           m_LastFramesTotal;
	BYTE 		  m_rgbBuffer[4*VIDEO_HEIGHT*VIDEO_WIDTH];
	BYTE 		  m_rgb_noalpa_Buffer[3*VIDEO_HEIGHT*VIDEO_WIDTH];	
	RGBQUAD		  m_rgbDepth[VIDEO_HEIGHT*VIDEO_WIDTH];
	USHORT		  m_depthBuffer[DEPTH_HEIGHT*DEPTH_WIDTH];
	USHORT		  m_playerBuffer[DEPTH_HEIGHT*DEPTH_WIDTH];

	//bool			newRGBData;
};