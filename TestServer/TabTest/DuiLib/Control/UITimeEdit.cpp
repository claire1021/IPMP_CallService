#include "stdafx.h"
#include "UIEdit.h"

namespace DuiLib
{


	class CTimeEditWnd : public CWindowWnd
	{
	public:
		CTimeEditWnd();

		void Init(CTimeEditUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		HWND GetHWND();

		void SetRange(CDuiString strRange);
		void SetENType(CUR_SELECT_TYPE enType);

		bool FilterAndModifyChar(UINT nChar,CDuiString &strTextData);

		void SelectSetTimeSel(POINT pt); //时间处理
		void SelectSetDateSel(POINT pt);//日期处理
		int GetDaysInMonth( int year,int month );//获取当前月的总天数

		CDuiString FormatDateAndtTimeForStandard(CDuiString strData);
		CDuiString FormatDateForStandard(CDuiString strData);
		CDuiString FormatTimeForStandard(CDuiString strData);

		void GetSelPos(OUT long &nL,OUT long &nR);

	protected:
		CTimeEditUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;


		CDuiString m_strRange;//记录允许的字符
		CUR_SELECT_TYPE m_enType;//记录当前选择的枚举类型

		CDuiString  m_strTextData ;//记录控件内容

		CUR_CTL_TYPE m_enCtlType;
	};


	CTimeEditWnd::CTimeEditWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false),m_strRange(_T("0123456789")),m_strTextData(_T("")),
		m_enCtlType(TYPE_BEGIN),m_enType(SELECT_BEGIN)
	{
	}

	void CTimeEditWnd::Init(CTimeEditUI* pOwner)
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
		::SetWindowLong(GetHWND(), GWL_STYLE, styleValue);
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		::SetFocus(m_hWnd);

		m_enCtlType = m_pOwner->GetCtlType();

		if(TYPE_DATE == m_enCtlType)
		{
			m_pOwner->SetSel(0,4);
			m_enType = SELECT_YEAR;
		}
		else if(TYPE_TIME == m_enCtlType)
		{
			m_pOwner->SetSel(0,2);
			m_enType = SELECT_HOUR;
		}
		HideCaret(m_hWnd);
		//ShowCursor(FALSE);
		m_bInit = true;    
	}

	RECT CTimeEditWnd::CalPos()
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

	LPCTSTR CTimeEditWnd::GetWindowClassName() const
	{
		return _T("TimeEditWnd");
	}

	LPCTSTR CTimeEditWnd::GetSuperClassName() const
	{
		return WC_EDIT;
	}

	void CTimeEditWnd::OnFinalMessage(HWND /*hWnd*/)
	{
		m_pOwner->Invalidate();
		// Clear reference and die
		if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CTimeEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if( uMsg == WM_KILLFOCUS ) 
		{
			m_strTextData = m_pOwner->GetText();
			if(m_strTextData.GetLength() > 0)
			{
				if(TYPE_DATE == m_enCtlType)
					m_pOwner->SetText(FormatDateForStandard(m_strTextData));
				else if(TYPE_TIME == m_enCtlType)
					m_pOwner->SetText(FormatTimeForStandard(m_strTextData));
			}
				lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
				
		}
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
			if( m_hBkBrush == NULL ) {
				DWORD clrColor = m_pOwner->GetNativeEditBkColor();
				m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
			}
			return (LRESULT)m_hBkBrush;
		}
		else if(uMsg == WM_CHAR)
		{
			m_strTextData = _T("");
			CDuiString strEditOld = m_pOwner->GetText();
			if(wParam == VK_BACK || wParam == VK_DELETE ||!FilterAndModifyChar(wParam,m_strTextData) )
			{
				//bHandled = FALSE;
				bHandled = TRUE;
				//m_pOwner->SetText(strEditOld);
				//keybd_event(VK_BACK,0,0,0);//错误数据删除
				//keybd_event(VK_BACK,0,KEYEVENTF_KEYUP,0);
				//return 0;
			}
			else
			{
				//bHandled = FALSE;
				//m_pOwner->SetText(FormatDateForStandard(strTextData));
				m_pOwner->SetText(m_strTextData);
				long nL = 0,nR = 0;
				GetSelPos(nL,nR );
				m_pOwner->SetSel(nL,nR);
			}
		}
		else if (uMsg == WM_KEYDOWN  || uMsg == WM_KEYUP)
		{
			
		


			//CDuiString str = m_pOwner->GetText();
		}
		else if(uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK)
		{
			 //m_pOwner->SetSel(0,4);
			POINT ptMose = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};
			if(TYPE_DATE == m_enCtlType)
				SelectSetDateSel( ptMose);
			else if(TYPE_TIME == m_enCtlType)
				SelectSetTimeSel(ptMose);
			 
		}
		else bHandled = FALSE;
		if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CTimeEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		PostMessage(WM_CLOSE);
		return lRes;
	}

	LRESULT CTimeEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if( !m_bInit ) return 0;
		if( m_pOwner == NULL ) return 0;
		// Copy text back
		int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
		LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
		ASSERT(pstr);
		if( pstr == NULL ) return 0;
		::GetWindowText(m_hWnd, pstr, cchLen);
		m_pOwner->m_sText = pstr;
		m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
		return 0;
	}

	HWND CTimeEditWnd::GetHWND()
	{
		return m_hWnd;
	}


	int CTimeEditWnd::GetDaysInMonth( int year,int month )//获取当前月的总天数
	{
		int days[]= {31,28,31,30,31,30,31,31,30,31,30,31};
		int day;
		if(2 == month)
		{
			day = (((0==year%4)&&(0!=year%100)||(0==year%400))?29:28);
		}
		else
		{
			day = days[month];
		}

		return day;
	}




		//************************************
	// 方法名:    SetRange
	// 全名:  CTimeEditUI::SetRange
	// 访问权限:    public 
	// 返回值:   void
	// 参数: CDuiString strRange
	//************************************

	void CTimeEditWnd::SetRange( CDuiString strRange )
	{
		m_strRange = strRange;
	}

	//************************************
	// 方法名:    SetENType
	// 全名:  CTimeEditUI::SetENType
	// 访问权限:    public 
	// 返回值:   void
	// 参数: CUR_SELECT_TYPE enType
	//************************************

	void CTimeEditWnd::SetENType( CUR_SELECT_TYPE enType )
	{
		m_enType = enType;
	}

	//************************************
	// 方法名:    FilterAndModifyChar
	// 全名:  CTimeEditUI::FilterAndModifyChar
	// 访问权限:    public 
	// 返回值:   void
	// 参数: char nChar
	//************************************

	bool CTimeEditWnd::FilterAndModifyChar( UINT nChar,CDuiString &strTextData)
	{
		CDuiString strIntToStr = _T("");
		strTextData = m_pOwner->GetText();
	
		if(nChar >= 48 && nChar <=57)//判断是否是数字
		{
			strIntToStr.Format(_T("%d"), nChar - 48);
		}
		else if(nChar>= 0x60 && nChar <= 0x69)//判断小键盘数字
		{
			strIntToStr.Format(_T("%d"), nChar - 0x60);
		}
		if(strIntToStr.GetLength() > 1 || 0 ==strIntToStr.GetLength() )//其他输入
		{		
			return false;
		}
		if(m_strRange.Find(strIntToStr) < 0)
		{
			return false;
		}

		long nR = 0;
		long nL = 0;

		GetSelPos(nL,nR);
	
		CDuiString strGetData =_T("");
		strGetData = strTextData.Mid(nL,nR - nL);

		int nullPos = - 2;
		nullPos = strGetData.Find(_T(" "));//是否有空格
		if(nullPos == -1 )//没有空格，输入一个字符，其它加入空格
		{
			//CString strNullData = _T("");
			strGetData = _T("");
			for (int j =1; j < nR - nL; j ++)
			{			
				strGetData += _T(" ");
			}


			if(SELECT_MONTH == m_enType || SELECT_DAY == m_enType)//月份最大为12
			{
				if(strIntToStr  ==  _T("0"))
				{
					strIntToStr = _T("1");
				}
			}
			strGetData += strIntToStr;

		}
		else//有空格
		{
			CDuiString tempStr = _T("");//删掉最前面一个空格
			for(int k = 0; k < strGetData.GetLength() - 1; k ++)
			{	
				tempStr += strGetData[k + 1];//得到少一个空格的字符串，替换当前输入的数字
			}
			if(SELECT_MONTH == m_enType)//月份最大为12
			{
				if((strIntToStr>=_T("1") && tempStr >=_T("2"))//大于12月 strIntToStr为当前输入的字符
					|| (tempStr == _T("1") && strIntToStr > _T("2")))
					tempStr = _T(" ");
				if(_T("0") == strIntToStr && tempStr >=_T("2"))//大于12月
				{
					strIntToStr = tempStr;
					tempStr = _T(" ");
				}
			}
			else if(SELECT_DAY == m_enType)
			{ 
				CDuiString strYear = strTextData.Mid(0,4);//取年
				CDuiString strMonth = strTextData.Mid(5,2);//取月
				int day = GetDaysInMonth(atoi(strYear),atoi(strMonth));//获取这个月的总天数
				CDuiString strDay =_T("");
				strDay.Format(_T("%d"),day);
				CDuiString strOne = strDay[0];
				CDuiString strTwo = strDay[1];
				if(tempStr >strOne ||( tempStr ==strOne && strIntToStr > strTwo))//大于当月最大天数
				{
					tempStr = _T(" ");
				}

			}
			else if(SELECT_HOUR == m_enType)
			{
				if(tempStr > _T("2") || (tempStr ==  _T("2")  && strIntToStr >=  _T("4") ))
					tempStr = _T(" ");
				
			}
			else if(SELECT_MINUTE == m_enType || SELECT_SECOND == m_enType)
			{
				if(tempStr >  _T("5")  )
				tempStr = _T(" ");
			}
			tempStr += strIntToStr;
			strGetData = tempStr;

		}
		CDuiString strData = _T("");//将所得数据进行替换
		for(int c = 0; c < strTextData.GetLength(); c ++)
		{

			if(c >= nL && c < nR )
			{
// 				if(strGetData[c - nL] == _T(' ') &&SELECT_YEAR  == m_enType )
// 				{
// 					strData +=  '1';
// 				}
// 				else if(strGetData[c - nL] ==_T(' ') &&(SELECT_MONTH  == m_enType || SELECT_DAY  == m_enType ))
// 				{
// 					strData +=   _T('0');
// 				}
// 				else 				
					strData += strGetData[c - nL];
			}

			else
			{
				strData += strTextData[c];
			}
			//m_newText.Replace(m_newText.Mid(nL,),strGetData);nR - nL
		}
		strTextData = strData;
		
		return true;
	}

	//************************************
	// 方法名:    SelectSetSel
	// 全名:  CTimeEditWnd::SelectSetSel
	// 访问权限:    public 
	// 返回值:   void
	// 参数: POINT pt
	//************************************

	void CTimeEditWnd::SelectSetDateSel( POINT pt )
	{
		HFONT nFont = m_pOwner->GetManager()->GetFont(m_pOwner->GetFont());//得到当前字体

		///::CClientDC dcClient(m_hWnd);
		HDC hHdc = ::GetDC(m_hWnd);//由句柄获取当前HDC
		HFONT nOldFont = SelectFont(hHdc,nFont);//选择当前字体
		
		SIZE sizeYear = {0};
		::GetTextExtentPoint32(hHdc,m_pOwner->GetText(),4,&sizeYear);

		SIZE sizeMon = {0};
		::GetTextExtentPoint32(hHdc,m_pOwner->GetText(),7,&sizeMon);

		SIZE sizeDay = {0};
		::GetTextExtentPoint32(hHdc,m_pOwner->GetText(),10,&sizeDay);

		SelectFont(hHdc,nOldFont);//恢复旧字体

		if(sizeYear.cx >= pt.x &&pt.x >= 0)//判断光标在年区域
		{
			//pEdit->SetFocus();
			if(m_strTextData.GetLength() > 0)
				m_pOwner->SetText(FormatDateForStandard(m_strTextData));
			m_pOwner->SetSel(0,4);
			m_enType = SELECT_YEAR;
			
			//pEdit1->SetFocus();
		}
		else if(pt.x > sizeYear.cx   && pt.x  <= sizeMon.cx)//判断光标在月区域
		{
			if(m_strTextData.GetLength() > 0)
				m_pOwner->SetText(FormatDateForStandard(m_strTextData));
			m_pOwner->SetSel(5,7);
			m_enType = SELECT_MONTH;
			
		}
		else if(pt.x > sizeMon.cx   && pt.x  <= sizeDay.cx)//判断光标在日区域
		{
			if(m_strTextData.GetLength() > 0)
				m_pOwner->SetText(FormatDateForStandard(m_strTextData));
			m_pOwner->SetSel(8,10);
			m_enType = SELECT_DAY;
			
		}

	}

	//************************************
	// 方法名:    FormatDateForStandard
	// 全名:  CTimeEditWnd::FormatDateForStandard
	// 访问权限:    public 
	// 返回值:   DuiLib::CDuiString
	// 参数: CDuiString strData
	//************************************

	DuiLib::CDuiString CTimeEditWnd::FormatDateAndtTimeForStandard( CDuiString strTextData )
	{
		


		if(TYPE_TIME == m_enCtlType)
			return FormatTimeForStandard( strTextData);
		else if(TYPE_DATE == m_enCtlType)
			return FormatDateForStandard( strTextData);		
		return _T("");
	}

	//************************************
	// 方法名:    SelectSetTimeSel
	// 全名:  CTimeEditWnd::SelectSetTimeSel
	// 访问权限:    public 
	// 返回值:   void
	// 参数: POINT pt
	//************************************

	void CTimeEditWnd::SelectSetTimeSel( POINT pt )
	{
		HFONT nFont = m_pOwner->GetManager()->GetFont(m_pOwner->GetFont());//得到当前字体

		///::CClientDC dcClient(m_hWnd);
		HDC hHdc = ::GetDC(m_hWnd);//由句柄获取当前HDC
		HFONT nOldFont = SelectFont(hHdc,nFont);//选择当前字体

		SIZE sizeHour = {0};
		::GetTextExtentPoint32(hHdc,m_pOwner->GetText(),2,&sizeHour);

		SIZE sizeMin = {0};
		::GetTextExtentPoint32(hHdc,m_pOwner->GetText(),5,&sizeMin);

		SIZE sizeSec = {0};
		::GetTextExtentPoint32(hHdc,m_pOwner->GetText(),8,&sizeSec);

		SelectFont(hHdc,nOldFont);//恢复旧字体

		if(sizeHour.cx >= pt.x &&pt.x >= 0)//判断光标在年区域
		{
			//pEdit->SetFocus();
			if(m_strTextData.GetLength() > 0)
				m_pOwner->SetText(FormatTimeForStandard(m_strTextData));
			m_pOwner->SetSel(0,2);
			m_enType = SELECT_HOUR;

			//pEdit1->SetFocus();
		}
		else if(pt.x > sizeHour.cx   && pt.x  <= sizeMin.cx)//判断光标在月区域
		{
			if(m_strTextData.GetLength() > 0)
				m_pOwner->SetText(FormatTimeForStandard(m_strTextData));
			m_pOwner->SetSel(3,5);
			m_enType = SELECT_MINUTE;

		}
		else if(pt.x > sizeMin.cx   && pt.x  <= sizeSec.cx)//判断光标在日区域
		{
			if(m_strTextData.GetLength() > 0)
				m_pOwner->SetText(FormatTimeForStandard(m_strTextData));
			m_pOwner->SetSel(6,8);
			m_enType = SELECT_SECOND;

		}

	}

	//************************************
	// 方法名:    FormatForStandard
	// 全名:  CTimeEditWnd::FormatForStandard
	// 访问权限:    public 
	// 返回值:   DuiLib::CDuiString
	// 参数: CDuiString strData
	//************************************

	DuiLib::CDuiString CTimeEditWnd::FormatDateForStandard( CDuiString strTextData )
	{
		CDuiString strData = _T("");//将所得数据进行替换

		for(int c = 0; c < strTextData.GetLength(); c ++)
		{

			//if(c >= nL && c < nR )
			
				if(strTextData[c] == _T(' ') && c < 4 )
				{
					strData +=  '1';
				}
				else if(strTextData[c] ==_T(' ') && c > 4)
				{
					strData +=   _T('0');
				}
				else 				
					strData += strTextData[c];
			

			//m_newText.Replace(m_newText.Mid(nL,),strGetData);nR - nL
		}

		return strData;
	}

	//************************************
	// 方法名:    FormatTimeForStandard
	// 全名:  CTimeEditWnd::FormatTimeForStandard
	// 访问权限:    public 
	// 返回值:   DuiLib::CDuiString
	// 参数: CDuiString strData
	//************************************

	DuiLib::CDuiString CTimeEditWnd::FormatTimeForStandard( CDuiString strTextData )
	{
		CDuiString strData = _T("");//将所得数据进行替换

		for(int c = 0; c < strTextData.GetLength(); c ++)
		{

			//if(c >= nL && c < nR )
			if(strTextData[c] ==_T(' '))
			{
				strData +=   _T('0');
			}
			else 				
				strData += strTextData[c];


			//m_newText.Replace(m_newText.Mid(nL,),strGetData);nR - nL
		}

		return strData;
	}

	//************************************
	// 方法名:    GetSelPos
	// 全名:  CTimeEditWnd::GetSelPos
	// 访问权限:    public 
	// 返回值:   void
	// 参数: OUT long & nL
	// 参数: OUT long & nR
	//************************************

	void CTimeEditWnd::GetSelPos( OUT long &nL,OUT long &nR )
	{
		if(SELECT_YEAR == m_enType)
		{
			nR = 4;
			nL = 0;
		}
		else if(SELECT_MONTH == m_enType)
		{
			nR = 7;
			nL = 5;
		}
		else if(SELECT_DAY == m_enType)
		{
			nR = 10;
			nL = 8;
		}
		else if(SELECT_HOUR == m_enType)
		{
			nR = 2;
			nL = 0;
		}
		else if(SELECT_MINUTE == m_enType)
		{
			nR = 5;
			nL = 3;
		}
		else if(SELECT_SECOND == m_enType)
		{
			nR = 8;
			nL = 6;
		}
	}






	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CTimeEditUI::CTimeEditUI() : m_pWindow(NULL), m_uMaxChar(255), m_bReadOnly(false), 
		m_bPasswordMode(false), m_cPasswordChar(_T('*')), m_uButtonState(0), 
		m_dwEditbkColor(0xFFFFFFFF), m_iWindowStyls(0),m_enCtlType(TYPE_BEGIN),//,m_test(0)
		m_bShowKeyboard(true)
	{
		SetTextPadding(CDuiRect(4, 3, 4, 3));
		SetBkColor(0xFFFFFFFF);

		
	}

	LPCTSTR CTimeEditUI::GetClass() const
	{
		return _T("EditUI");
	}

	LPVOID CTimeEditUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_EDIT) == 0 ) return static_cast<CTimeEditUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CTimeEditUI::GetControlFlags() const
	{
		if( !IsEnabled() ) return CControlUI::GetControlFlags();

		return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
	}

	void CTimeEditUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
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
			m_pWindow = new CTimeEditWnd();
			ASSERT(m_pWindow);
			m_pWindow->Init(this);
			//int dd = m_test;
			Invalidate();
			
			//Edit_SetSel(*m_pWindow, 0, 4);
		}
		if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
		{
			if( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if( IsFocused() && m_pWindow == NULL )
				{
					m_pWindow = new CTimeEditWnd();
					ASSERT(m_pWindow);
					m_pWindow->Init(this);

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
#if 0
					int nSize = GetWindowTextLength(*m_pWindow);
					if( nSize == 0 )
						nSize = 1;

					Edit_SetSel(*m_pWindow, 0, nSize);
#else
					POINT pt = event.ptMouse;
					pt.x -= m_rcItem.left + m_rcTextPadding.left;
					pt.y -= m_rcItem.top + m_rcTextPadding.top;
					::SendMessage(*m_pWindow, WM_LBUTTONUP, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
				}
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE ) 
		{
			//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) 
		{
			
			return;
		}
		if(event.Type == UIEVENT_KEYDOWN)
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

	void CTimeEditUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	void CTimeEditUI::SetText(LPCTSTR pstrText)
	{
		m_sText = pstrText;
		if( m_pWindow != NULL ) Edit_SetText(*m_pWindow, m_sText);
		Invalidate();
	}

	void CTimeEditUI::SetMaxChar(UINT uMax)
	{
		m_uMaxChar = uMax;
		if( m_pWindow != NULL ) Edit_LimitText(*m_pWindow, m_uMaxChar);
	}

	UINT CTimeEditUI::GetMaxChar()
	{
		return m_uMaxChar;
	}

	void CTimeEditUI::SetReadOnly(bool bReadOnly)
	{
		if( m_bReadOnly == bReadOnly ) return;

		m_bReadOnly = bReadOnly;
		if( m_pWindow != NULL ) Edit_SetReadOnly(*m_pWindow, m_bReadOnly);
		Invalidate();
	}

	bool CTimeEditUI::IsReadOnly() const
	{
		return m_bReadOnly;
	}

	void CTimeEditUI::SetNumberOnly(bool bNumberOnly)
	{
		if( bNumberOnly )
		{
			m_iWindowStyls |= ES_NUMBER;
		}
		else
		{
			m_iWindowStyls |= ~ES_NUMBER;
		}
	}

	bool CTimeEditUI::IsNumberOnly() const
	{
		return m_iWindowStyls&ES_NUMBER ? true:false;
	}

	int CTimeEditUI::GetWindowStyls() const 
	{
		return m_iWindowStyls;
	}

	void CTimeEditUI::SetPasswordMode(bool bPasswordMode)
	{
		if( m_bPasswordMode == bPasswordMode ) return;
		m_bPasswordMode = bPasswordMode;
		Invalidate();
	}

	bool CTimeEditUI::IsPasswordMode() const
	{
		return m_bPasswordMode;
	}

	void CTimeEditUI::SetPasswordChar(TCHAR cPasswordChar)
	{
		if( m_cPasswordChar == cPasswordChar ) return;
		m_cPasswordChar = cPasswordChar;
		if( m_pWindow != NULL ) Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
		Invalidate();
	}

	TCHAR CTimeEditUI::GetPasswordChar() const
	{
		return m_cPasswordChar;
	}

	LPCTSTR CTimeEditUI::GetNormalImage()
	{
		return m_diNormal.sDrawString;
	}

	void CTimeEditUI::SetNormalImage(LPCTSTR pStrImage)
	{
		if( m_diNormal.sDrawString == pStrImage && m_diNormal.pImageInfo != NULL ) return;
		m_diNormal.Clear();
		m_diNormal.sDrawString = pStrImage;
		Invalidate();
	}

	LPCTSTR CTimeEditUI::GetHotImage()
	{
		return m_diHot.sDrawString;	
	}

	void CTimeEditUI::SetHotImage(LPCTSTR pStrImage)
	{
		if( m_diHot.sDrawString == pStrImage && m_diHot.pImageInfo != NULL ) return;
		m_diHot.Clear();
		m_diHot.sDrawString = pStrImage;
		Invalidate();
	}

	LPCTSTR CTimeEditUI::GetFocusedImage()
	{
		return m_diFocused.sDrawString;	
	}

	void CTimeEditUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		if( m_diFocused.sDrawString == pStrImage && m_diFocused.pImageInfo != NULL ) return;
		m_diFocused.Clear();
		m_diFocused.sDrawString = pStrImage;
		Invalidate();
	}

	LPCTSTR CTimeEditUI::GetDisabledImage()
	{
		return m_diDisabled.sDrawString;	
	}

	void CTimeEditUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		if( m_diDisabled.sDrawString == pStrImage && m_diDisabled.pImageInfo != NULL ) return;
		m_diDisabled.Clear();
		m_diDisabled.sDrawString = pStrImage;
		Invalidate();
	}

	void CTimeEditUI::SetNativeEditBkColor(DWORD dwBkColor)
	{
		m_dwEditbkColor = dwBkColor;
	}

	DWORD CTimeEditUI::GetNativeEditBkColor() const
	{
		return m_dwEditbkColor;
	}

	void CTimeEditUI::SetSel(long nStartChar, long nEndChar)
	{
		if( m_pWindow != NULL ) Edit_SetSel(*m_pWindow, nStartChar,nEndChar);
	}

	void CTimeEditUI::SetSelAll()
	{
		SetSel(0,-1);
	}

	void CTimeEditUI::SetReplaceSel(LPCTSTR lpszReplace)
	{
		if( m_pWindow != NULL ) Edit_ReplaceSel(*m_pWindow, lpszReplace);
	}

	void CTimeEditUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CTimeEditUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		CControlUI::Move(szOffset, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CTimeEditUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	void CTimeEditUI::SetInternVisible(bool bVisible)
	{
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	SIZE CTimeEditUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) return CDuiSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 6);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CTimeEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
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
		else if(_tcscmp(pstrName,_T("style")) == 0)
		{
			if(_tcscmp(pstrValue,_T("date")) == 0)
				m_enCtlType = TYPE_DATE;
			else if(_tcscmp(pstrValue,_T("time")) == 0)
				m_enCtlType = TYPE_TIME;
			else
				m_enCtlType = TYPE_DATE;

			InitTimeToCtl();
		}
		else if(_tcscmp(pstrName, _T("showkeyboard")) == 0) ShowKeyBoard(_tcscmp(pstrValue, _T("true")) == 0);
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CTimeEditUI::PaintStatusImage(HDC hDC)
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

	void CTimeEditUI::PaintText(HDC hDC)
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

	//************************************
	// 方法名:    GetCtlType
	// 全名:  CTimeEditUI::GetCtlType
	// 访问权限:    public 
	// 返回值:   int
	//************************************

	CUR_CTL_TYPE CTimeEditUI::GetCtlType()
	{
		return m_enCtlType;
	}

// 	HWND CTimeEditUI::GetHWND()
// 	{
// 		if(!m_pWindow)
// 			return NULL;
// 		return m_pWindow->GetHWND();
// 	}

	void CTimeEditUI::ShowKeyBoard( bool bShow )
	{
		m_bShowKeyboard = bShow;
	}


	SYSTEMTIME CTimeEditUI::GetTime()
	{
		CDuiString str =_T("");
		str = GetText();
		SYSTEMTIME pst;
		memset(&pst, 0, sizeof(SYSTEMTIME));
		::GetLocalTime(&pst);

		if(TYPE_DATE == m_enCtlType)
		{
			pst.wYear = _tstoi(str.Left(4));
			pst.wMonth = _tstoi(str.Mid(5,2));
			pst.wDay = _tstoi(str.Right(2)); 
		}
		else if(TYPE_TIME == m_enCtlType)
		{
			 pst.wHour = _tstoi(str.Left(2));
			 pst.wMinute = _tstoi(str.Mid(3,2));
			 pst.wSecond = _tstoi(str.Right(2));
		}
		return pst;
	}

	//************************************
	// 方法名:    InitTimeToCtl
	// 全名:  CTimeEditUI::InitTimeToCtl
	// 访问权限:    public 
	// 返回值:   void
	//************************************

	void CTimeEditUI::InitTimeToCtl()
	{
		SYSTEMTIME sysTime = {0};
		CDuiString strTime = _T("");
		::GetLocalTime(&sysTime);
		if(TYPE_DATE == m_enCtlType)
		{
			strTime.SmallFormat(_T("%4d-%02d-%02d"),
				sysTime.wYear, sysTime.wMonth, sysTime.wDay);
		}
		else if(TYPE_TIME == m_enCtlType)
		{ 
			strTime.SmallFormat(_T("%02d:%02d:%02d"),
				sysTime.wHour, sysTime.wMinute,sysTime.wSecond);
		}
		SetText(strTime);
	}

	void CTimeEditUI::SetTime(SYSTEMTIME* pst)
	{
		SYSTEMTIME sysTime = {0};
		CDuiString strTime = _T("");
	
		if(TYPE_DATE == m_enCtlType)
		{
			strTime.SmallFormat(_T("%4d-%02d-%02d"),
				pst->wYear, pst->wMonth, pst->wDay);
		}
		else if(TYPE_TIME == m_enCtlType)
		{ 
			strTime.SmallFormat(_T("%02d:%02d:%02d"),
				pst->wHour, pst->wMinute,pst->wSecond);
		}
		SetText(strTime);
	}
}
