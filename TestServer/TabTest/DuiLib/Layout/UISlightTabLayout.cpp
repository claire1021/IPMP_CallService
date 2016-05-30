#include "stdafx.h"
#include "UISlightTabLayout.h"

namespace DuiLib {
	class CSlightWindow: public CWindowWnd{
	public:
		CSlightWindow(CSlightTabLayout * pOwner)
		{
			m_pOwner = pOwner;
		}
		~CSlightWindow(){}
		LPCTSTR GetWindowClassName() const
		{
			return _T("FadeWindow");
		}
		 LPCTSTR GetSuperClassName() const
		{
			return NULL;
		}
		void OnFinalMessage(HWND hWnd)
		{
			m_pOwner->Invalidate(); 
			m_pOwner->m_pWindow = NULL;
			delete this;
		} 
		 LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		 {
			 LRESULT ret =  __super::HandleMessage(uMsg, wParam,lParam);
			 if(uMsg == WM_PAINT)
			 {
				   HDC hDC = GetDC(m_hWnd); 
				   RECT rc;
				   GetClientRect(m_hWnd, &rc);
				   ::BitBlt(hDC, rc.left, rc.top, rc.right, rc.bottom, m_pOwner->m_hMenDC, 0,0,SRCCOPY);
				   ReleaseDC(m_hWnd,hDC);    
			 } 
			 return ret;
		 }
	private:
		CSlightTabLayout * m_pOwner;
	};




	CSlightTabLayout::CSlightTabLayout() :   
		m_pCurrentControl( NULL ),
		m_bControlVisibleFlag( false ),
		m_hMenDC(NULL),
		m_pWindow(NULL),
		m_hBitmap(NULL)
	{
	}

	CSlightTabLayout::~CSlightTabLayout()
	{

	}

	LPCTSTR CSlightTabLayout::GetClass() const
	{
		return _T("SlightTabLayout");
	}

	LPVOID CSlightTabLayout::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_SLIGHTTABLAYOUT) == 0 ) 
			return static_cast<CSlightTabLayout*>(this);
		return CTabLayoutUI::GetInterface(pstrName);
	}

	bool CSlightTabLayout::SelectItem( int iIndex )
	{ 
		if( iIndex < 0 || iIndex >= m_items.GetSize() ) return false;
		if( iIndex == m_iCurSel ) return true; 

		int iOldSel = m_iCurSel;
		m_iCurSel = iIndex;
		HWND hWnd = GetManager()->GetPaintWindow();
		AnimationSwitch(); 
		for( int it = 0; it < m_items.GetSize(); it++ )
		{
			if( it == iIndex ) {
				
				GetItemAt(it)->SetVisible(true);
				GetItemAt(it)->SetFocus();   
			}
			else GetItemAt(it)->SetVisible(false); 
		} 

		if( m_pManager != NULL ) {
			m_pManager->SetNextTabControl();
			m_pManager->SendNotify(this, DUI_MSGTYPE_TABSELECT, m_iCurSel, iOldSel);
		}
		return true;
	} 

	bool CSlightTabLayout::SelectItem( CControlUI * pControl )
	{
		int nIndex = GetItemIndex(pControl);
		return SelectItem(nIndex);
	}

	void CSlightTabLayout::AnimationSwitch()
	{ 
 		HWND hWnd = GetManager()->GetPaintWindow();

		HDC hDC = GetDC(hWnd);

		if(m_hMenDC == NULL)
			m_hMenDC = CreateCompatibleDC(hDC);  
		if(m_hBitmap == NULL)
			m_hBitmap = ::CreateCompatibleBitmap(hDC, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
		m_OldBitmap=(HBITMAP)::SelectObject(m_hMenDC,m_hBitmap);    //把位图选进内存DC   

		::BitBlt(m_hMenDC, 0,0,m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top,
			hDC,m_rcItem.left, m_rcItem.top, SRCCOPY  ); 
		//::SelectObject(m_hMenDC, OldBitmap);
		::ReleaseDC(hWnd, hDC) ; 
		if(m_pWindow == NULL)
		{ 
			m_pWindow = new CSlightWindow(this);
			m_pWindow->Create(hWnd,_T(""), WS_VISIBLE | WS_POPUP, WS_EX_LAYERED);
		}    
		::SetWindowPos(m_pWindow->GetHWND(), hWnd, m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.bottom,0);   
		::SetLayeredWindowAttributes(m_pWindow->GetHWND(), 0, 255, LWA_ALPHA);
		m_pWindow->ShowWindow(true);
		m_pWindow->SendMessage( WM_PAINT, NULL, NULL);  
		m_Trans = 255; 
// 		StopAnimation( TAB_ANIMATION_ID );
// 		StartAnimation( TAB_ANIMATION_ELLAPSE, TAB_ANIMATION_FRAME_COUNT, TAB_ANIMATION_ID );
		  CreateThread(NULL, 0,SlightProc, this, 0, NULL);
		
	}

 

	void CSlightTabLayout::OnAnimationStep(INT nTotalFrame, INT nCurFrame)
	{ 
		 m_Trans -= 255/TAB_ANIMATION_FRAME_COUNT;
		 if(m_Trans < 0)
			 m_Trans = 0;
		if(m_pWindow != NULL )
		{
			::SetLayeredWindowAttributes(m_pWindow->GetHWND(), 0, m_Trans, LWA_ALPHA);
		}
		Invalidate();

	}

	void CSlightTabLayout::OnAnimationStop() 
	{
		::SelectObject(m_hMenDC, m_OldBitmap);
		DeleteObject(m_OldBitmap);
		DeleteDC(m_hMenDC);
		DeleteObject(m_hBitmap);
		m_hMenDC = NULL;
		m_OldBitmap = NULL;
		m_hBitmap = NULL; 
		if(m_pWindow != NULL)
			m_pWindow->Close();
		NeedParentUpdate();
	}
 
 

	void CSlightTabLayout::OnFinalMessage( HWND hWnd )
	{
		if(m_hMenDC != NULL)
		{
			DeleteDC(m_hMenDC);
			m_hMenDC = NULL;
		}   
	}

	DWORD WINAPI CSlightTabLayout::SlightProc( LPVOID pParam )
	{
		for(int i = 1; i < TAB_ANIMATION_FRAME_COUNT; i++)
		{
			((CSlightTabLayout *) pParam)->OnAnimationStep(TAB_ANIMATION_FRAME_COUNT,i);
			Sleep(TAB_ANIMATION_ELLAPSE);
		}
		((CSlightTabLayout *) pParam)->OnAnimationStop();
		return 0;
	}

} // namespace DuiLib