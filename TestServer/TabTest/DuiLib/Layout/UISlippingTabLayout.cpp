#include "stdafx.h"
#include "UISlippingTabLayout.h"

namespace DuiLib {
	CSlippingTabLayoutUI::CSlippingTabLayoutUI() : 
		//CUIAnimation( this ), 
		m_bIsVerticalDirection( false ), 
		m_nPositiveDirection( 1 ),
		m_pCurrentControl( NULL ),
		m_bControlVisibleFlag( false )
	{
	}

	CSlippingTabLayoutUI::~CSlippingTabLayoutUI()
	{

	}

	LPCTSTR CSlippingTabLayoutUI::GetClass() const
	{
		return _T("SlippingTabLayoutUI");
	}

	LPVOID CSlippingTabLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_SLIPPINGTABLAYOUT) == 0 ) 
			return static_cast<CSlippingTabLayoutUI*>(this);
		return CTabLayoutUI::GetInterface(pstrName);
	}

	bool CSlippingTabLayoutUI::SelectItem( int iIndex )
	{
		if( iIndex < 0 || iIndex >= m_items.GetSize() ) return false;
		if( iIndex == m_iCurSel ) return true;
		if( iIndex > m_iCurSel ) m_nPositiveDirection = -1;
		if( iIndex < m_iCurSel ) m_nPositiveDirection = 1;

		int iOldSel = m_iCurSel;
		m_iCurSel = iIndex;
		for( int it = 0; it < m_items.GetSize(); it++ )
		{
			if( it == iIndex ) {
				GetItemAt(it)->SetVisible(true);
				GetItemAt(it)->SetFocus();

				m_bControlVisibleFlag = false;
				m_pCurrentControl = static_cast<CControlUI*>(m_items[it]);
				if( NULL != m_pCurrentControl )
					m_pCurrentControl->SetVisible( false );
			}
			else GetItemAt(it)->SetVisible(false);
		}
		//NeedParentUpdate(); 
		AnimationSwitch();
		if( m_pManager != NULL ) {
			m_pManager->SetNextTabControl();
			m_pManager->SendNotify(this, _T("tabselect"), m_iCurSel, iOldSel);
		}
		return true;
	}

	bool CSlippingTabLayoutUI::SelectItem( CControlUI * pControl )
	{
		int nIndex = GetItemIndex(pControl);
		return SelectItem(nIndex);
	}

	void CSlippingTabLayoutUI::AnimationSwitch()
	{
		m_rcItemOld = m_rcItem;
		if( !m_bIsVerticalDirection )
		{
			m_rcCurPos.top = m_rcItem.top;
			m_rcCurPos.bottom = m_rcItem.bottom;
			m_rcCurPos.left = m_rcItem.left - ( m_rcItem.right - m_rcItem.left ) * m_nPositiveDirection /*+ 52 * m_nPositiveDirection*/;
			m_rcCurPos.right = m_rcItem.right - ( m_rcItem.right - m_rcItem.left ) * m_nPositiveDirection/*+ 52 * m_nPositiveDirection*/;	 
		}
		else
		{
			m_rcCurPos.left = m_rcItem.left;
			m_rcCurPos.right = m_rcItem.right;
			m_rcCurPos.top = m_rcItem.top - ( m_rcItem.bottom - m_rcItem.top ) * m_nPositiveDirection;
			m_rcCurPos.bottom = m_rcItem.bottom - ( m_rcItem.bottom - m_rcItem.top ) * m_nPositiveDirection;		
		}
		 
		CreateThread(NULL, 0,SlippingProc, this, 0, NULL); 
	}
// 
// 	void CSlippingTabLayoutUI::DoEvent(TEventUI& event)
// 	{
// 		if( event.Type == UIEVENT_TIMER ) 
// 		{
// 			OnTimer(  event.wParam );
// 		}
// 		__super::DoEvent( event );
// 	}

// 	void CSlippingTabLayoutUI::OnTimer( int nTimerID )
// 	{
// 		OnAnimationElapse( nTimerID );
// 	}

	void CSlippingTabLayoutUI::OnAnimationStep(INT nTotalFrame, INT nCurFrame)
	{
		if( !m_bControlVisibleFlag )
		{
			m_bControlVisibleFlag = true;
			m_pCurrentControl->SetVisible( true );
		}

		int iStepLen = 0;
		if( !m_bIsVerticalDirection )
		{
			iStepLen = ( m_rcItemOld.right - m_rcItemOld.left ) * m_nPositiveDirection / nTotalFrame;
			if( nCurFrame != nTotalFrame )
			{
				m_rcCurPos.left = m_rcCurPos.left + iStepLen;
				m_rcCurPos.right = m_rcCurPos.right +iStepLen;			
			}
			else
			{
				m_rcItem = m_rcCurPos = m_rcItemOld;
			}
		}
		else
		{
			iStepLen = ( m_rcItemOld.bottom - m_rcItemOld.top ) * m_nPositiveDirection / nTotalFrame;
			if( nCurFrame != nTotalFrame )
			{
				m_rcCurPos.top = m_rcCurPos.top + iStepLen;
				m_rcCurPos.bottom = m_rcCurPos.bottom +iStepLen;			
			}
			else
			{
				m_rcItem = m_rcCurPos = m_rcItemOld;	
			}	
		}
		SetPos(m_rcCurPos);
	}
	 

	void CSlippingTabLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("animation_direction")) == 0 && _tcscmp( pstrValue, _T("vertical")) == 0 ) m_bIsVerticalDirection = true; // pstrValue = "vertical" or "horizontal"
		return CTabLayoutUI::SetAttribute(pstrName, pstrValue);
	}

	DWORD WINAPI CSlippingTabLayoutUI::SlippingProc( LPVOID pParam )
	{
		Sleep(20);
		for(int i = 1; i < TAB_ANIMATION_FRAME_COUNT; i++)
		{
			((CSlippingTabLayoutUI *) pParam)->OnAnimationStep(TAB_ANIMATION_FRAME_COUNT,i);
			Sleep(TAB_ANIMATION_ELLAPSE);
		}
 		((CSlippingTabLayoutUI *) pParam)->NeedParentUpdate();
		return 0;
	}

} // namespace DuiLib