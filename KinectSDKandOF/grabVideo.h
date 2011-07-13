/*test*/
#pragma once
//#ifndef GRAB_VIDEO_H
//#define GRAB_VIDEO_H

#include <tchar.h>
#include <strsafe.h>
#include <conio.h>

#include "resource.h"

//#define SZ_APPDLG_WINDOW_CLASS        _T("SkeletalViewerAppDlgWndClass")

#define KINECT_WIDTH 640;
#define KINECT_HEIGHT 480;

class VideoGrabber
{
public:
    HRESULT Video_Init();
    void                    Video_UnInit( );
    void                    Video_GotDepthAlert( );
    void                    Video_GotVideoAlert( );
    void                    Video_GotSkeletonAlert( );
    void                    Video_Zero();
	int						Video_Update();
	void					print_bytes();
	int						getImageWidth();
	int						getImageHeight();
	BYTE*					getAlphaPixels();
	BYTE*                   getDepthPixels();
    //void                    Nui_BlankSkeletonScreen( HWND hWnd );
    //void                    Nui_DoDoubleBuffer(HWND hWnd,HDC hDC);
    //void                    Nui_DrawSkeleton( bool bBlank, NUI_SKELETON_DATA * pSkel, HWND hWnd, int WhichSkeletonColor );
    //void                    Nui_DrawSkeletonSegment( NUI_SKELETON_DATA * pSkel, int numJoints, ... );

    RGBQUAD                 Nui_ShortToQuad_Depth( USHORT s );

    //static LONG CALLBACK    WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    //HWND m_hWnd;

private:
    //static DWORD WINAPI     Video_ProcessThread(LPVOID pParam);


    // NUI and draw stuff
    //DrawDevice    m_DrawDepth;
    //DrawDevice    m_DrawVideo;

    // thread handling
    HANDLE        m_hThVideoProcess;
    HANDLE        m_hEvVideoProcessStop;

    HANDLE        m_hNextDepthFrameEvent;
	HANDLE        m_pDepthStreamHandle;
    HANDLE        m_hNextVideoFrameEvent;
    HANDLE        m_hNextSkeletonEvent;
    HANDLE        m_pVideoStreamHandle;
    //HFONT         m_hFontFPS;
    //HPEN          m_Pen[6];
    //HDC           m_SkeletonDC;
    //HBITMAP       m_SkeletonBMP;
    //HGDIOBJ       m_SkeletonOldObj;
    //int           m_PensTotal;
    //POINT         m_Points[NUI_SKELETON_POSITION_COUNT];
    RGBQUAD       m_rgbWk[640*480];
    //int           m_LastSkeletonFoundTime;
    bool          m_bScreenBlanked;
    int           m_FramesTotal;
    //int           m_LastFPStime;
    int           m_LastFramesTotal;
	BYTE *		  pBuffer; //r,g,b,alpha of each pixel

};
