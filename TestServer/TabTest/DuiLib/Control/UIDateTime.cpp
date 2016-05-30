#include "stdafx.h"
#include "UIDateTime.h"

namespace DuiLib
{
	//CDateTimeUI::m_nDTUpdateFlag
#define DT_NONE   0
#define DT_UPDATE 1
#define DT_DELETE 2
#define DT_KEEP   3

	class CDateTimeWnd : public CWindowWnd
	{
	public:
		CDateTimeWnd();

		void Init(CDateTimeUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		//LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	protected:
		CDateTimeUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
	};

	CDateTimeWnd::CDateTimeWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false)
	{
	}

	void CDateTimeWnd::Init(CDateTimeUI* pOwner)
	{
		m_pOwner = pOwner;
		m_pOwner->m_nDTUpdateFlag = DT_NONE;

		if (m_hWnd == NULL)
		{
			RECT rcPos = CalPos();
			UINT uStyle =WS_CHILD|m_pOwner->m_dwStyle;// �޸��ߣ�YYM
			Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
			SetWindowFont(m_hWnd, m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->hFont, TRUE);
			//lxm,added,�޸�Date�ؼ�����Ϊ����
			if(m_pOwner->m_dwStyle == DTS_SHORTDATECENTURYFORMAT || m_pOwner->m_dwStyle == DTS_SHORTDATEFORMAT)
			{ 
				CDuiString strFormat = _T("yyyy-MM-dd");
				::SendMessage(m_hWnd, DTM_SETFORMAT, 0, (LPARAM)strFormat.GetData());
			}
		}

		if (m_pOwner->GetText().IsEmpty())
			::GetLocalTime(&m_pOwner->m_sysTime);

		::SendMessage(m_hWnd, DTM_SETSYSTEMTIME, 0, (LPARAM)&m_pOwner->m_sysTime);
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		::SetFocus(m_hWnd);
// 
// 		SYSTEMTIME sysTimes[2]; 
// 		GetLocalTime(&(sysTimes[0]));  
// 		DateTime_SetRange(m_hWnd,GDTR_MIN,sysTimes ); 
		m_bInit = true;    
	}

	RECT CDateTimeWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		return rcPos;
	}

	LPCTSTR CDateTimeWnd::GetWindowClassName() const
	{
		return _T("DateTimeWnd");
	}

	LPCTSTR CDateTimeWnd::GetSuperClassName() const
	{
		return DATETIMEPICK_CLASS;
	}

	void CDateTimeWnd::OnFinalMessage(HWND /*hWnd*/)
	{
		// Clear reference and die
		if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CDateTimeWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if( uMsg == WM_KILLFOCUS )
		{
			lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
		}
		else if (uMsg == WM_KEYUP && (wParam == VK_DELETE || wParam == VK_BACK))
		{
			LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
			m_pOwner->m_nDTUpdateFlag = DT_DELETE;
			m_pOwner->UpdateText();
			PostMessage(WM_CLOSE);
			return lRes;
		}
		else if (uMsg == WM_KEYUP && wParam == VK_ESCAPE)
		{
			LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
			m_pOwner->m_nDTUpdateFlag = DT_KEEP;
			PostMessage(WM_CLOSE);
			return lRes;
		}
		//	else if( uMsg == OCM_COMMAND ) {
		// 		if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
		// 		else if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE ) {
		// 			RECT rcClient;
		// 			::GetClientRect(m_hWnd, &rcClient);
		// 			::InvalidateRect(m_hWnd, &rcClient, FALSE);
		// 		}
		//	}
		//	else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN ) {
		// 		m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);
		//	}
		// 		else if( uMsg == OCM__BASE + WM_CTLCOLOREDIT  || uMsg == OCM__BASE + WM_CTLCOLORSTATIC ) {
		// 			if( m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF ) return NULL;
		// 			::SetBkMode((HDC)wParam, TRANSPARENT);
		// 			DWORD dwTextColor = m_pOwner->GetTextColor();
		// 			::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor)));
		// 			if( m_hBkBrush == NULL ) {
		// 				DWORD clrColor = m_pOwner->GetNativeEditBkColor();
		// 				m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
		// 			}
		// 			return (LRESULT)m_hBkBrush;
		// 		}
		else bHandled = FALSE;
		if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CDateTimeWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		if (m_pOwner->m_nDTUpdateFlag == DT_NONE)
		{
			::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)&m_pOwner->m_sysTime);
			m_pOwner->m_nDTUpdateFlag = DT_UPDATE;
			m_pOwner->UpdateText();
		}
		PostMessage(WM_CLOSE);

		return lRes;
	}

	// LRESULT CDateTimeWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	// {
	// 	if( !m_bInit ) return 0;
	// 	if( m_pOwner == NULL ) return 0;
	// 	// Copy text back
	// 	int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
	// 	LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
	// 	ASSERT(pstr);
	// 	if( pstr == NULL ) return 0;
	// 	::GetWindowText(m_hWnd, pstr, cchLen);
	// 	m_pOwner->m_sText = pstr;
	// 	m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
	// 	return 0;
	// }

	//////////////////////////////////////////////////////////////////////////
	//
	CDateTimeUI::CDateTimeUI():
		m_bShowKeyboard(true)
	{
		m_dwStyle = DTS_SHORTDATEFORMAT;
		::GetLocalTime(&m_sysTime);
		m_bReadOnly = false;
		m_pWindow = NULL;
		m_nDTUpdateFlag=DT_UPDATE;
		UpdateText();		// add by:daviyang35 ��ʼ������ʱ��ʾʱ��
		m_nDTUpdateFlag = DT_NONE;
		
	}

	LPCTSTR CDateTimeUI::GetClass() const
	{
		return _T("DateTimeUI");
	}

	LPVOID CDateTimeUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_DATETIME) == 0 ) return static_cast<CDateTimeUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	SYSTEMTIME& CDateTimeUI::GetTime()
	{
		return m_sysTime;
	}

	void CDateTimeUI::SetTime(SYSTEMTIME* pst)
	{
		m_sysTime = *pst;
		m_nDTUpdateFlag = DT_UPDATE;
		UpdateText();
		Invalidate();
	}

	void CDateTimeUI::SetReadOnly(bool bReadOnly)
	{
		m_bReadOnly = bReadOnly;
		Invalidate();
	}

	bool CDateTimeUI::IsReadOnly() const
	{
		return m_bReadOnly;
	}

	void CDateTimeUI::UpdateText()
	{
		if (m_nDTUpdateFlag == DT_DELETE)
			SetText(_T(""));
		else if (m_nDTUpdateFlag == DT_UPDATE)
		{// �޸��ߣ�YYM ʱ�䣺2015-7-22
			CDuiString sText;
			if ((m_dwStyle == DTS_SHORTDATEFORMAT) || (m_dwStyle == DTS_SHORTDATECENTURYFORMAT))
			{//��ʾ������
				sText.SmallFormat(_T("%4d-%02d-%02d"),
					m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay);
			}
			else if (m_dwStyle == DTS_TIMEFORMAT)
			{// ��ʾʱ��
				sText.SmallFormat(_T("%02d:%02d:%02d"),
					m_sysTime.wHour, m_sysTime.wMinute,m_sysTime.wSecond);
			}
			else
			{// ��ʾ������
				sText.SmallFormat(_T("%4d��%02d��%02d��"),
					m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay);
			}
		
			SetText(sText);
		}
	}

	void CDateTimeUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			return;
		}
		if( event.Type == UIEVENT_WINDOWSIZE )
		{
			if( m_pWindow != NULL ) m_pManager->SetFocusNeeded(this);
		}
		if( event.Type == UIEVENT_SCROLLWHEEL )
		{
			if( m_pWindow != NULL ) return;
		}
		if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
		{
			if(m_bShowKeyboard)
				m_pManager->ShowKeyBoard(true, this);
			if( m_pWindow ) return;
			m_pWindow = new CDateTimeWnd();
			ASSERT(m_pWindow);
			m_pWindow->Init(this);
			m_pWindow->ShowWindow(); 
	
		}
		if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
		{
			m_pManager->ShowKeyBoard(false);
			Invalidate();
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
		{
			if( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if( IsFocused() && m_pWindow == NULL )
				{
					m_pWindow = new CDateTimeWnd();
					ASSERT(m_pWindow);
				}
				if( m_pWindow != NULL )
				{
					m_pWindow->Init(this);
					m_pWindow->ShowWindow();
				}
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE ) 
		{
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) 
		{
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			return;
		}

		CLabelUI::DoEvent(event);
	}

	void CDateTimeUI::SetTimeStyle()
	{
		m_nDTUpdateFlag = DT_UPDATE;
		m_dwStyle = DTS_TIMEFORMAT;
		UpdateText();
		Invalidate();
	}

	void CDateTimeUI::SetShortDataStyle()
	{
		m_nDTUpdateFlag = DT_UPDATE;
		m_dwStyle = DTS_SHORTDATEFORMAT;
		UpdateText();
		Invalidate();
	}

	void CDateTimeUI::SetShortCenturDataStyle()
	{
		m_nDTUpdateFlag = DT_UPDATE;
		m_dwStyle = DTS_SHORTDATECENTURYFORMAT;
		UpdateText();
		Invalidate();
	}

	void CDateTimeUI::SetLongDataStyle()
	{
		
		m_nDTUpdateFlag = DT_UPDATE;
		m_dwStyle = DTS_LONGDATEFORMAT;
		UpdateText();
		Invalidate();
	}

	void CDateTimeUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{ 
		if(_tcscmp(pstrName, _T("style")) == 0)
		{
			if(_tcscmp(pstrValue, _T("shortdata")) == 0)
			{
				 SetShortDataStyle();
			}
			else if(_tcscmp(pstrValue, _T("shortcenturdata")) == 0)
			{
				SetShortCenturDataStyle();
			} 
			else if(_tcscmp(pstrValue, _T("longdata")) == 0)
			{
				SetLongDataStyle();
			} 
			else if(_tcscmp(pstrValue, _T("time")) == 0)
			{
				SetTimeStyle();
			} 
		}
		else if(_tcscmp(pstrName, _T("showkeyboard")) == 0) ShowKeyBoard(_tcscmp(pstrValue, _T("true")) == 0);
		__super::SetAttribute(pstrName, pstrValue);
	}

	void CDateTimeUI::ShowKeyBoard( bool bShow )
	{
		m_bShowKeyboard = bShow;
	}

}
