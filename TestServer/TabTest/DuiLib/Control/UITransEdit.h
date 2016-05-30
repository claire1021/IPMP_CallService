/*
作者：lxm
时间：2015年7月15日16:55:51
说明：在CUIEdit基础上修改
//初始不能设置文字，因为需要抓背景，不可以把原文字抓上，所以不支持初始化设置文字功能。
//如果在显示出界面后想设置文字，可调用SetText
//另外，界面初始化完成后，不能随意更改背景或挪动位置（父窗口为纯色除外），因为已经抓取了父窗口背景
*/
#ifndef __UITRANS_EDIT_H__
#define __UITRANS_EDIT_H__

#pragma once 

namespace DuiLib
{ 
	class UILIB_API CTransEditUI:public CLabelUI
	{
		friend class CTransEditWnd;
	public:
		CTransEditUI();
		~CTransEditUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetEnabled(bool bEnable = true);
		void SetText(LPCTSTR pstrText);
		void SetMaxChar(UINT uMax);
		UINT GetMaxChar();
		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;
		void SetPasswordMode(bool bPasswordMode);
		bool IsPasswordMode() const;
		void SetPasswordChar(TCHAR cPasswordChar);
		TCHAR GetPasswordChar() const;
		void SetNumberOnly(bool bNumberOnly);
		bool IsNumberOnly() const;
		int GetWindowStyls() const;
		 
		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		void SetNativeEditBkColor(DWORD dwBkColor);
		DWORD GetNativeEditBkColor() const;

		void SetSel(long nStartChar, long nEndChar);
		void SetSelAll();
		void SetReplaceSel(LPCTSTR lpszReplace);

		//lxm,2016年1月8日，获取当前光标位置
		DWORD GetSel();

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		SIZE EstimateSize(SIZE szAvailable);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);
		//lxm,added,2015年8月29日
		void ShowKeyBoard(bool bShow);
	protected:
		CTransEditWnd* m_pWindow;

		UINT m_uMaxChar;
		bool m_bReadOnly;
		bool m_bPasswordMode;
		TCHAR m_cPasswordChar;
		UINT m_uButtonState;
		DWORD m_dwEditbkColor;
		int m_iWindowStyls;

		TDrawInfo m_diNormal;
		TDrawInfo m_diHot;
		TDrawInfo m_diFocused;
		TDrawInfo m_diDisabled;

		//保存edit下的父窗口图片
		HBRUSH m_hBkBrush;
		//lxm,added,2015年8月29日,是否需要软键盘，默认为true
		bool m_bShowKeyboard;
		//Edit失去焦点是用于记录Edit焦点位置
		DWORD m_dwSel;

	
	};
		 
}





#endif //__UITRANS_EDIT_H__