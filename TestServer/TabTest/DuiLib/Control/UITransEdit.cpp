/*
作者：lxm
时间：2015年7月15日16:55:51
说明：在CUIEdit基础上修改
//初始不能设置文字，因为需要抓背景，不可以把原文字抓上，所以不支持初始化设置文字功能。
//如果在显示出界面后想设置文字，可调用SetText
//另外，界面初始化完成后，不能随意更改背景或挪动位置（父窗口为纯色除外），因为已经抓取了父窗口背景
*/
#include "stdafx.h"
#include "UITransEdit.h"


namespace DuiLib
{
	class CTransEditWnd : public CWindowWnd
	{
	public:
		CTransEditWnd();

		void Init(CTransEditUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	protected:
		CTransEditUI* m_pOwner;
		bool m_bInit;
			string m_strlast;
	};

	CTransEditWnd::CTransEditWnd() : m_pOwner(NULL), m_bInit(false)
	{
	}

	void CTransEditWnd::Init(CTransEditUI* pOwner)
	{
		m_pOwner = pOwner;
		RECT rcPos = CalPos();
		UINT uStyle = WS_CHILD | ES_AUTOHSCROLL;
		if( m_pOwner->IsPasswordMode() ) uStyle |= ES_PASSWORD;
		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
		HFONT hFont=NULL;
		int iFontIndex=m_pOwner->GetFont();
		if (iFontIndex!=-1)
			hFont=m_pOwner->GetManager()->GetFont(iFontIndex);
		if (hFont==NULL)
			hFont=m_pOwner->GetManager()->GetDefaultFontInfo()->hFont;

		SetWindowFont(m_hWnd, hFont, TRUE);
		Edit_LimitText(m_hWnd, m_pOwner->GetMaxChar());
		if( m_pOwner->IsPasswordMode() ) Edit_SetPasswordChar(m_hWnd, m_pOwner->GetPasswordChar());
		Edit_SetText(m_hWnd, m_pOwner->GetText());
		Edit_SetModify(m_hWnd, FALSE);
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
		Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);
		Edit_SetReadOnly(m_hWnd, m_pOwner->IsReadOnly() == true);
		//Styls
		LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
		styleValue |= pOwner->GetWindowStyls();
		//lxm,added
		//|pOwner->GetTextStyle(),为了设置对其方式
		::SetWindowLong(GetHWND(), GWL_STYLE, styleValue|pOwner->GetTextStyle());
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		::SetFocus(m_hWnd);
		m_bInit = true;    
	
	}

	RECT CTransEditWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		RECT rcInset = m_pOwner->GetTextPadding();
		rcPos.left += rcInset.left;
		rcPos.top += rcInset.top;
		rcPos.right -= rcInset.right;
		rcPos.bottom -= rcInset.bottom;
		LONG lEditHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
		if( lEditHeight < rcPos.GetHeight() ) {
			rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
			rcPos.bottom = rcPos.top + lEditHeight;
		}
		return rcPos;
	}

	LPCTSTR CTransEditWnd::GetWindowClassName() const
	{
		return _T("TransEditWnd");
	}

	LPCTSTR CTransEditWnd::GetSuperClassName() const
	{
		return WC_EDIT;
	}

	void CTransEditWnd::OnFinalMessage(HWND /*hWnd*/)
	{
 		m_pOwner->Invalidate();
		// Clear reference and die
		//if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CTransEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE; 
		if( uMsg == WM_KILLFOCUS) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
		else if( uMsg == OCM_COMMAND ) {
			if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
			else if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE ) {
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
			}
		}
		else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN ) {
			m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);
		} 
		else if( uMsg == OCM__BASE + WM_CTLCOLOREDIT  || uMsg == OCM__BASE + WM_CTLCOLORSTATIC ) {
			if( m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF ) return NULL;
			::SetBkMode((HDC)wParam, TRANSPARENT); 
			DWORD dwTextColor = m_pOwner->GetTextColor();
			::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor))); 
			if( m_pOwner->m_hBkBrush == NULL ) {
				DWORD clrColor = m_pOwner->GetNativeEditBkColor(); 
				//获取父窗口背景  
				HWND hParent = GetParent(m_hWnd); 
				tagRECT rcParent, rcThis;
				GetWindowRect(hParent, &rcParent); 
				GetWindowRect(m_hWnd, &rcThis); 
				Rect rcClient(rcThis.left - rcParent.left, rcThis.top - rcParent.top,
					rcThis.right - rcThis.left, rcThis.bottom - rcThis.top);  
				HDC hDC = GetDC(hParent); 
				HDC hMemDC=CreateCompatibleDC(hDC);   
				HBITMAP bitmap = ::CreateCompatibleBitmap(hDC, rcClient.Width, rcClient.Height);
				HBITMAP OldBitmap=(HBITMAP)::SelectObject(hMemDC,bitmap);    //把位图选进内存DC   
			
				::BitBlt(hMemDC, 0,0,rcClient.Width, rcClient.Height, hDC, rcClient.X, rcClient.Y, SRCCOPY);

				m_pOwner->m_hBkBrush = CreatePatternBrush(bitmap);
				::SelectObject(hMemDC,OldBitmap);
				DeleteObject(bitmap);
				DeleteDC(hMemDC);
				ReleaseDC(hParent, hDC);   
			}
			return (LRESULT)m_pOwner->m_hBkBrush;
		}
		else bHandled = FALSE;
		if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CTransEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam); 
		if(m_pOwner != NULL) m_pOwner->m_dwSel= Edit_GetSel(GetHWND()); 
  		PostMessage(WM_CLOSE);
		return lRes;
	}

	LRESULT CTransEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if( !m_bInit ) return 0;
		if( m_pOwner == NULL ) return 0;
		// Copy text back
		int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
		LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
		ASSERT(pstr);
		if( pstr == NULL ) return 0;
		::GetWindowText(m_hWnd, pstr, cchLen); 
		if((cchLen - 1)*2 > (int) m_pOwner->GetMaxChar())
		{
			int nMutLen = 0;
#if _UNICODE 
			nMutLen = WideCharToMultiByte(CP_ACP,0, pstr, cchLen - 1, NULL, 0, NULL, FALSE);
#else
			nMutLen = strlen(pstr);
			if(nMutLen >(int)( m_pOwner->GetMaxChar()))
			{ 
				//获取光标位置 
				int nPos = LOWORD(Edit_GetSel(m_hWnd));   

				if (0 == strlen(m_strlast.c_str()))
				{  
					int count = 0;
					size_t i = 0;
					for (i = 0; i<m_pOwner->GetMaxChar(); i++)
					{
						if ((pstr[i]&0x80) == 0x80)
							count++;
					}
					if (0 == count%2)  
						pstr[m_pOwner->GetMaxChar()] = '\0';  
					else 
						pstr[m_pOwner->GetMaxChar() -1] = '\0'; 
					m_strlast = pstr ;   
				}
				Edit_SetText(m_hWnd, m_strlast.c_str());
				m_pOwner->m_sText = m_strlast.c_str();
				m_pOwner->SetSel(nPos -1, nPos -1);
			} 
			else
			{ 
				m_strlast = "";
				if(nMutLen == m_pOwner->GetMaxChar())
				{
					m_strlast = pstr;   
				}  
					m_pOwner->m_sText = pstr; 
			}
#endif
		} 
		else
		{ 
			m_pOwner->m_sText = pstr; 
		}
		m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
		return 0;
	}



	CTransEditUI::CTransEditUI() : m_pWindow(NULL), m_uMaxChar(255), m_bReadOnly(false), 
		m_bPasswordMode(false), m_cPasswordChar(_T('*')), m_uButtonState(0), 
		m_dwEditbkColor(0x00FFFFF), m_iWindowStyls(0), m_hBkBrush(NULL),
		m_bShowKeyboard(true)
	{
		SetTextPadding(CDuiRect(4, 3, 4, 3)); 
	}

	CTransEditUI::~CTransEditUI()
	{
		if(m_pWindow != NULL)
			PostMessage(m_pWindow->GetHWND(), WM_CLOSE, 0, 0);
		if(m_hBkBrush != NULL)
		{
			DeleteObject(m_hBkBrush);
			m_hBkBrush = NULL;
		}
	}


	LPCTSTR CTransEditUI::GetClass() const
	{
		return _T("CTransEditUI");
	}

	LPVOID CTransEditUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_TRANS_EDIT) == 0 ) return static_cast<CTransEditUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CTransEditUI::GetControlFlags() const
	{
		if( !IsEnabled() ) return CControlUI::GetControlFlags();

		return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
	}

	void CTransEditUI::DoEvent(TEventUI& event)
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
			if( m_pWindow == NULL) 
			{ 
				m_pWindow = new CTransEditWnd();
				ASSERT(m_pWindow);
				m_pWindow->Init(this);
				if(m_bShowKeyboard)
					m_pManager->ShowKeyBoard(true, this);
				SetSelAll();
			}
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
					m_pWindow = new CTransEditWnd();
					ASSERT(m_pWindow); 
					m_pWindow->Init(this);
					SetSelAll();
// 					if( PtInRect(&m_rcItem, event.ptMouse) )
// 					{
// 						int nSize = GetWindowTextLength(*m_pWindow);
// 						if( nSize == 0 )
// 							nSize = 1;
// 
// 						Edit_SetSel(*m_pWindow, 0, nSize);
// 					}
				} 
				else if( m_pWindow != NULL )
				{
#if 1 
// 					int nSize = GetWindowTextLength(*m_pWindow);
// 					if( nSize == 0 )
// 						nSize = 1;
// 
// 					Edit_SetSel(*m_pWindow, 0, nSize);
					SetSelAll();
#else
					POINT pt = event.ptMouse;
					pt.x -= m_rcItem.left + m_rcTextPadding.left;
					pt.y -= m_rcItem.top + m_rcTextPadding.top;
					::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
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
			if( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}
	 
		 
		CLabelUI::DoEvent(event);
	}

	void CTransEditUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	void CTransEditUI::SetText(LPCTSTR pstrText)
	{
		m_sText = pstrText;
		if( m_pWindow != NULL ) Edit_SetText(*m_pWindow, m_sText);
		if(m_hBkBrush == NULL)
		{ 
			DWORD clrColor = GetNativeEditBkColor(); 
			//获取父窗口背景     
			CDuiRect rcEdit = GetPos();

			RECT rcInset = GetTextPadding();
			rcEdit.left += rcInset.left;
			rcEdit.top += rcInset.top;
			rcEdit.right -= rcInset.right;
			rcEdit.bottom -= rcInset.bottom;

			LONG lEditHeight = GetManager()->GetFontInfo(GetFont())->tm.tmHeight;
			if( lEditHeight < rcEdit.GetHeight() ) {
				rcEdit.top += (rcEdit.GetHeight() - lEditHeight) / 2;
				rcEdit.bottom = rcEdit.top + lEditHeight;
			}
			HDC hDC = GetDC(GetManager()->GetPaintWindow()); 
			HDC hMemDC=CreateCompatibleDC(hDC);   
			HBITMAP bitmap = ::CreateCompatibleBitmap(hDC, rcEdit.GetWidth(), rcEdit.GetHeight());
			HBITMAP OldBitmap=(HBITMAP)::SelectObject(hMemDC,bitmap);    //把位图选进内存DC   

			::BitBlt(hMemDC, 0,0,rcEdit.GetWidth(), rcEdit.GetHeight(), hDC, rcEdit.left, rcEdit.top, SRCCOPY);

			m_hBkBrush = CreatePatternBrush(bitmap);
			::SelectObject(hMemDC,OldBitmap);
			DeleteObject(bitmap);
			DeleteDC(hMemDC);
			ReleaseDC(GetManager()->GetPaintWindow(), hDC); 
		}  
		Invalidate();
	}

	void CTransEditUI::SetMaxChar(UINT uMax)
	{
		m_uMaxChar = uMax;
		if( m_pWindow != NULL ) Edit_LimitText(*m_pWindow, m_uMaxChar);
	}

	UINT CTransEditUI::GetMaxChar()
	{
		return m_uMaxChar;
	}

	void CTransEditUI::SetReadOnly(bool bReadOnly)
	{
		if( m_bReadOnly == bReadOnly ) return;

		m_bReadOnly = bReadOnly;
		if( m_pWindow != NULL ) Edit_SetReadOnly(*m_pWindow, m_bReadOnly);
		Invalidate();
	}

	bool CTransEditUI::IsReadOnly() const
	{
		return m_bReadOnly;
	}

	void CTransEditUI::SetNumberOnly(bool bNumberOnly)
	{
		if( bNumberOnly )
		{
			m_iWindowStyls |= ES_NUMBER;
		}
		else
		{
			m_iWindowStyls &= ~ES_NUMBER;
		}
	}

	bool CTransEditUI::IsNumberOnly() const
	{
		return m_iWindowStyls&ES_NUMBER ? true:false;
	}

	int CTransEditUI::GetWindowStyls() const 
	{
		return m_iWindowStyls;
	}

	void CTransEditUI::SetPasswordMode(bool bPasswordMode)
	{
		if( m_bPasswordMode == bPasswordMode ) return;
		m_bPasswordMode = bPasswordMode;
		Invalidate();
	}

	bool CTransEditUI::IsPasswordMode() const
	{
		return m_bPasswordMode;
	}

	void CTransEditUI::SetPasswordChar(TCHAR cPasswordChar)
	{
		if( m_cPasswordChar == cPasswordChar ) return;
		m_cPasswordChar = cPasswordChar;
		if( m_pWindow != NULL ) Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
		Invalidate();
	}

	TCHAR CTransEditUI::GetPasswordChar() const
	{
		return m_cPasswordChar;
	}

	LPCTSTR CTransEditUI::GetNormalImage()
	{
		return m_diNormal.sDrawString;
	}

	void CTransEditUI::SetNormalImage(LPCTSTR pStrImage)
	{
		if( m_diNormal.sDrawString == pStrImage && m_diNormal.pImageInfo != NULL ) return;
		m_diNormal.Clear();
		m_diNormal.sDrawString = pStrImage;
		Invalidate();
	}

	LPCTSTR CTransEditUI::GetHotImage()
	{
		return m_diHot.sDrawString;	
	}

	void CTransEditUI::SetHotImage(LPCTSTR pStrImage)
	{
		if( m_diHot.sDrawString == pStrImage && m_diHot.pImageInfo != NULL ) return;
		m_diHot.Clear();
		m_diHot.sDrawString = pStrImage;
		Invalidate();
	}

	LPCTSTR CTransEditUI::GetFocusedImage()
	{
		return m_diFocused.sDrawString;	
	}

	void CTransEditUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		if( m_diFocused.sDrawString == pStrImage && m_diFocused.pImageInfo != NULL ) return;
		m_diFocused.Clear();
		m_diFocused.sDrawString = pStrImage;
		Invalidate();
	}

	LPCTSTR CTransEditUI::GetDisabledImage()
	{
		return m_diDisabled.sDrawString;	
	}

	void CTransEditUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		if( m_diDisabled.sDrawString == pStrImage && m_diDisabled.pImageInfo != NULL ) return;
		m_diDisabled.Clear();
		m_diDisabled.sDrawString = pStrImage;
		Invalidate();
	}

	void CTransEditUI::SetNativeEditBkColor(DWORD dwBkColor)
	{
		m_dwEditbkColor = dwBkColor;
	}

	DWORD CTransEditUI::GetNativeEditBkColor() const
	{
		return m_dwEditbkColor;
	}

	void CTransEditUI::SetSel(long nStartChar, long nEndChar)
	{
		if( m_pWindow != NULL ) Edit_SetSel(*m_pWindow, nStartChar,nEndChar);
	}

	void CTransEditUI::SetSelAll()
	{
		SetSel(0,-1);
	}

	void CTransEditUI::SetReplaceSel(LPCTSTR lpszReplace)
	{
		if( m_pWindow != NULL ) Edit_ReplaceSel(*m_pWindow, lpszReplace);
	}

	DWORD CTransEditUI::GetSel()
	{
		if( m_pWindow != NULL )return (LOWORD(Edit_GetSel(*(m_pWindow))));
		else return m_dwSel;
	}


	void CTransEditUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CTransEditUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		CControlUI::Move(szOffset, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CTransEditUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	void CTransEditUI::SetInternVisible(bool bVisible)
	{
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	SIZE CTransEditUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) return CDuiSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 6);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CTransEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		
		if(_tcscmp(pstrName, _T("text")) == 0) return;

		if( _tcscmp(pstrName, _T("readonly")) == 0 ) SetReadOnly(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("numberonly")) == 0 ) SetNumberOnly(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("password")) == 0 ) SetPasswordMode(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("maxchar")) == 0 ) SetMaxChar(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcscmp(pstrName, _T("nativebkcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetNativeEditBkColor(clrColor);
		}
			else if(_tcscmp(pstrName, _T("showkeyboard")) == 0) ShowKeyBoard(_tcscmp(pstrValue, _T("true")) == 0);
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CTransEditUI::PaintStatusImage(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if( DrawImage(hDC, m_diDisabled) ) return;
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( DrawImage(hDC, m_diFocused) ) return;
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( DrawImage(hDC, m_diHot) ) return;
		}

		if( DrawImage(hDC, m_diNormal) ) return;
	}

	void CTransEditUI::PaintText(HDC hDC)
	{  
		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if( m_sText.IsEmpty() ) return;

		CDuiString sText = m_sText;
		if( m_bPasswordMode ) {
			sText.Empty();
			LPCTSTR p = m_sText.GetData();
			while( *p != _T('\0') ) {
				sText += m_cPasswordChar;
				p = ::CharNext(p);
			}
		}

		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;
		if( IsEnabled() ) {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
		}
		else {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);

		}
	}

	void CTransEditUI::ShowKeyBoard( bool bShow )
	{
		m_bShowKeyboard = bShow;
	}


}
