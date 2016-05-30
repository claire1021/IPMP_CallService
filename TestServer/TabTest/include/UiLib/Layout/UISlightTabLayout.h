#ifndef __UISLIGHTTABLAYOUT_H__
#define __UISLIGHTTABLAYOUT_H__

#pragma once

#include "UIAnimation.h"

using namespace std;
namespace DuiLib
{

	class CSlightWindow;
	class UILIB_API CSlightTabLayout : public CTabLayoutUI
	{
	public:
		CSlightTabLayout();
		~CSlightTabLayout();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool SelectItem( int iIndex );
		bool SelectItem(CControlUI * pControl);
		void AnimationSwitch(); 
		virtual void OnFinalMessage(HWND hWnd);
		 
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame); 
		 
		void OnAnimationStop();
		static DWORD WINAPI SlightProc ( LPVOID pParam );
		CSlightWindow * m_pWindow;

		HDC m_hMenDC; 
	protected: 
		HBITMAP m_hBitmap;
		HBITMAP m_OldBitmap;
		 
		RECT m_rcCurPos;
		RECT m_rcItemOld;
		CControlUI* m_pCurrentControl;
		bool m_bControlVisibleFlag;
		enum
		{  
			TAB_ANIMATION_ELLAPSE = 20,
			TAB_ANIMATION_FRAME_COUNT = 20,
		};
		int m_Trans;
	};
}
#endif // __UISLIGHTTABLAYOUT_H__