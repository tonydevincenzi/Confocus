#pragma once
//#ifndef GRAB_VIDEO_H
//#define GRAB_VIDEO_H

//#include "MSR_NuiApi.h"
#include <tchar.h>
#include <strsafe.h>
#include <conio.h>
#include "resource.h"
#include <iostream>
#include <vector>
#include <process.h>

//#define SZ_APPDLG_WINDOW_CLASS        _T("SkeletalViewerAppDlgWndClass")

const int VIDEO_WIDTH = 640;
const int VIDEO_HEIGHT= 480;
const int DEPTH_WIDTH = 320;
const int DEPTH_HEIGHT= 240;


class KinectGrabber
{
public:
    HRESULT Kinect_Init();
    void                    Kinect_UnInit( );
    void                    Kinect_GotVideoAlert( );
	void                    Kinect_GotDepthAlert( );
    void                    Kinect_GotSkeletonAlert( );
    void                    Kinect_Zero();
	int						Kinect_Update();
	void					print_bytes();
	RGBQUAD*				Kinect_getDepthPixels();
	BYTE*					getAlphaPixels();
	void                    getJointsPoints();
	CRITICAL_SECTION*		Kinect_getRGBLock();
    //void                    Nui_BlankSkeletonScreen( HWND hWnd );
    //void                    Nui_DoDoubleBuffer(HWND hWnd,HDC hDC);
    //void                    Nui_DrawSkeleton( bool bBlank, NUI_SKELETON_DATA * pSkel);
    //void                    Nui_DrawSkeletonSegment( NUI_SKELETON_DATA * pSkel, int numJoints, ... );

    //RGBQUAD                 Nui_ShortToQuad_Depth( USHORT s );

    //static LONG CALLBACK    WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    //HWND m_hWnd;
	int           headJoints_x;
	int           headJoints_y;

private:
    //static DWORD WINAPI     Kinect_ProcessThread(LPVOID pParam);


    // NUI and draw stuff
    //DrawDevice    m_DrawDepth;
    //DrawDevice    m_DrawVideo;

	void		  Kinect_FormatRGBForOutput();
	RGBQUAD       Kinect_DepthToRGB( USHORT s );
    // thread handling
    //HANDLE        m_hThVideoProcess;
    //HANDLE        m_hEvVideoProcessStop;
    HANDLE        m_hNextSkeletonFrameEvent;
    HANDLE        m_hNextDepthFrameEvent;
    HANDLE        m_hNextVideoFrameEvent;
    HANDLE        m_hNextSkeletonEvent;
    HANDLE        m_pDepthStreamHandle;
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
    bool          m_bScreenBlanked;
    int           m_FramesTotal;
    //int           m_LastFPStime;
    int           m_LastFramesTotal;
	BYTE 		  m_rgbBuffer[4*VIDEO_HEIGHT*VIDEO_WIDTH];
	RGBQUAD		  m_rgbDepth[VIDEO_HEIGHT*VIDEO_WIDTH];
	USHORT		  m_depthBuffer[VIDEO_HEIGHT*VIDEO_WIDTH];
	USHORT		  m_playerBuffer[VIDEO_HEIGHT*VIDEO_WIDTH];

	CRITICAL_SECTION rgbLock;
	bool			newRGBData;
};
