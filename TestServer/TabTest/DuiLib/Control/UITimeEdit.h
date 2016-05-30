#ifndef __UITIMEEDIT_H__
#define __UITIMEEDIT_H__

#pragma once

namespace DuiLib
{
	class CTimeEditWnd;

	enum CUR_SELECT_TYPE
	{
		SELECT_BEGIN = 0,
		SELECT_YEAR = 1,
		SELECT_MONTH,
		SELECT_DAY,
		SELECT_HOUR,
		SELECT_MINUTE,
		SELECT_SECOND,
		SELECT_END
	};
	enum CUR_CTL_TYPE
	{
		TYPE_BEGIN = 0,
		TYPE_DATE,
		TYPE_TIME,
		TYPE_END
	};
	

	class UILIB_API CTimeEditUI : public CLabelUI
	{
		friend class CTimeEditWnd;
	public:
		CTimeEditUI();

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

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		SIZE EstimateSize(SIZE szAvailable);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);

		CUR_CTL_TYPE GetCtlType();
		void ShowKeyBoard(bool bShow);
		//HWND GetHWND();
		//int m_test;
		SYSTEMTIME GetTime();
		void InitTimeToCtl();
		void SetTime(SYSTEMTIME* pst);
	protected:
		CTimeEditWnd* m_pWindow;

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

		CUR_CTL_TYPE m_enCtlType;//记录时间控件类型：日期，时间

		bool m_bShowKeyboard; 

	};
}
#endif // __UITIMEEDIT_H__