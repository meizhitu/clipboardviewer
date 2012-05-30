//////////////////////////////////////////////////////////////////////////
// Name:	    PictureBox Header
// Version:		1.0
// Date:	    24-7-2004
// Author:	    Trilobyte
// Copyright:   (c) Trilobyte-Solutions 2004-2008
// Desciption:
// Defines CPictureBox and CMemDC in the WTL namespace
//////////////////////////////////////////////////////////////////////////
#ifndef __PICTUREBOX_H
#define __PICTUREBOX_H
#pragma once
#ifndef __ATLCRACK_H__
#error please include atlcrack.h first
#endif

#ifndef __ATLCTRLS_H__
#include <atlctrls.h>
#endif

#ifndef __ATLSCRL_H__
#include <atlscrl.h>
#endif

#ifndef __ATLMISC_H__
#define _ATL_TMP_NO_CSTRING
#include <atlmisc.h>
#endif

#ifndef __ATLIMAGE_H_
#define __ATLTYPES_H__	// Use the WTL types
#include <atlimage.h>
#endif

namespace WTL
{
	// CMemDC is an assistant for drawing
#ifndef _MEMDC
#define _MEMDC
	class CMemDC: public CDC
	{
	public:
		CDCHandle     m_hOwnerDC;	// Owner DC
		CBitmap       m_bitmap;		// Offscreen bitmap
		CBitmapHandle m_hOldBitmap;	// Originally selected bitmap
		RECT          m_rcOwner;	// Rectangle of drawing area

		CMemDC(HDC hDC, LPRECT pRect = NULL)
		{
			ATLASSERT(hDC!=NULL);
			m_hOwnerDC = hDC;
			if( pRect != NULL )
				m_rcOwner = *pRect; 
			else
				m_hOwnerDC.GetClipBox(&m_rcOwner);

			CreateCompatibleDC(m_hOwnerDC);
			::LPtoDP(m_hOwnerDC, (LPPOINT) &m_rcOwner, sizeof(RECT)/sizeof(POINT));
			m_bitmap.CreateCompatibleBitmap(m_hOwnerDC, m_rcOwner.right - m_rcOwner.left, m_rcOwner.bottom - m_rcOwner.top);
			m_hOldBitmap = SelectBitmap(m_bitmap);
			::DPtoLP(m_hOwnerDC, (LPPOINT) &m_rcOwner, sizeof(RECT)/sizeof(POINT));
			SetWindowOrg(m_rcOwner.left, m_rcOwner.top);
		}
		~CMemDC()
		{
			// Copy the offscreen bitmap onto the screen.
			m_hOwnerDC.BitBlt(m_rcOwner.left, m_rcOwner.top, m_rcOwner.right - m_rcOwner.left, m_rcOwner.bottom - m_rcOwner.top, m_hDC, m_rcOwner.left, m_rcOwner.top, SRCCOPY);
			//Swap back the original bitmap.
			SelectBitmap(m_hOldBitmap);
		}
	};
#endif
    // Picture box Styles and class
#define PICTUREBOX_MENU_CENTER	WM_APP + 1
#define PICTUREBOX_MENU_STRETCH	WM_APP + 2
	enum PICTUREBOX
	{
		PICTUREBOX_CENTER	= 0x0001,
		PICTUREBOX_STRETCH	= 0x0002,
		PICTUREBOX_MENU		= 0x0004,
		PICTUREBOX_OWNER	= 0x0008
	};
	class CPictureBox:	public WTL::CScrollWindowImpl<CPictureBox>
	{
	public:
		// Constructor/Destructor
				CPictureBox()
				{
					m_hBitmap = NULL;
				}
				~CPictureBox()
				{
				}
		// ATL Message map stuff
		DECLARE_WND_CLASS(NULL)
		BEGIN_MSG_MAP(CPictureBox)
			MSG_WM_CREATE(OnCreate)
			MSG_WM_ERASEBKGND(OnEraseBkGnd)
			CHAIN_MSG_MAP(WTL::CScrollWindowImpl<CPictureBox>)
			CHAIN_MSG_MAP_ALT(WTL::CScrollWindowImpl<CPictureBox>, 1)
			DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
		// Message handlers
		LRESULT	OnCreate(LPCREATESTRUCT)
		{
			SetMsgHandled(false);
			return 0;
		}
		LRESULT	OnEraseBkGnd(HDC)
		{
			return 0;
		}
		
		// Overloaded from CScrollWindowImpl<>
		void	DoPaint(CDCHandle hDC)
		{
			CMemDC		mDC(hDC.m_hDC);
			CRect	rcClient;
			POINT		ptScrollOffset;

			GetClientRect(&rcClient);
			GetScrollOffset(ptScrollOffset);
			rcClient.OffsetRect(ptScrollOffset);

			mDC.FillRect(&rcClient, AtlGetStockBrush(WHITE_BRUSH));

			if (m_hBitmap != NULL)
			{
				HDC   memdc; 
				memdc=CreateCompatibleDC(hDC.m_hDC); 
				HBITMAP oldBitmap = (HBITMAP)SelectObject(memdc,m_hBitmap); 
				mDC.BitBlt(rcClient.left,rcClient.top,rcClient.Height(),rcClient.Width(),memdc,0,0,SRCCOPY);
				SelectObject(memdc,oldBitmap);
				ReleaseDC(memdc);
				
			}
		}
		// Picture functions
		HBITMAP	GetBitmap()
		{
			return m_hBitmap;
		}
		void SetHBitmap(HBITMAP bitmap)
		{
			m_hBitmap = bitmap;
			Invalidate(FALSE);
		}

		HBITMAP m_hBitmap;

	};
}
#endif