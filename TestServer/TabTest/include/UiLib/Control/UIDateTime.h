#ifndef __UIDATETIME_H__
#define __UIDATETIME_H__

#pragma once

namespace DuiLib
{
	class CDateTimeWnd;

	/// ʱ��ѡ��ؼ�
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
		// ���������ؼ���ʾ��� �޸��ߣ�YYM  ʱ�䣺2015-7-22
			void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetShortDataStyle();
		void SetShortCenturDataStyle();
		void SetLongDataStyle();
		void SetUserFormat(CDuiString strFormat);
		void SetTimeStyle();
		//lxm,added,2015��8��29��
		void ShowKeyBoard(bool bShow);
	protected:
		SYSTEMTIME m_sysTime;
		int        m_nDTUpdateFlag;
		bool       m_bReadOnly;
		DWORD      m_dwStyle;
		CDateTimeWnd* m_pWindow;
		//lxm,added,2015��8��29��,�Ƿ���Ҫ����̣�Ĭ��Ϊtrue
		bool m_bShowKeyboard; 
	};
}
#endif // __UIDATETIME_H__