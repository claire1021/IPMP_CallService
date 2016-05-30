#ifndef __UIDATETIME_H__
#define __UIDATETIME_H__

#pragma once

namespace DuiLib
{
	class CDateTimeWnd;

	/// 时间选择控件
	class UILIB_API CDateTimeUI : public CLabelUI
	{
		friend class CDateTimeWnd;
	public:
		CDateTimeUI();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		SYSTEMTIME& GetTime();
		void SetTime(SYSTEMTIME* pst);

		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;

		void UpdateText();

		void DoEvent(TEventUI& event);
		// 设置日历控件显示风格 修改者：YYM  时间：2015-7-22
			void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetShortDataStyle();
		void SetShortCenturDataStyle();
		void SetLongDataStyle();
		void SetUserFormat(CDuiString strFormat);
		void SetTimeStyle();
		//lxm,added,2015年8月29日
		void ShowKeyBoard(bool bShow);
	protected:
		SYSTEMTIME m_sysTime;
		int        m_nDTUpdateFlag;
		bool       m_bReadOnly;
		DWORD      m_dwStyle;
		CDateTimeWnd* m_pWindow;
		//lxm,added,2015年8月29日,是否需要软键盘，默认为true
		bool m_bShowKeyboard; 
	};
}
#endif // __UIDATETIME_H__