/*
���ߣ�lxm
ʱ�䣺2015��7��15��16:55:51
˵������CUIEdit�������޸�
//��ʼ�����������֣���Ϊ��Ҫץ�����������԰�ԭ����ץ�ϣ����Բ�֧�ֳ�ʼ���������ֹ��ܡ�
//�������ʾ����������������֣��ɵ���SetText
//���⣬�����ʼ����ɺ󣬲���������ı�����Ų��λ�ã�������Ϊ��ɫ���⣩����Ϊ�Ѿ�ץȡ�˸����ڱ���
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

		//lxm,2016��1��8�գ���ȡ��ǰ���λ��
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
		//lxm,added,2015��8��29��
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

		//����edit�µĸ�����ͼƬ
		HBRUSH m_hBkBrush;
		//lxm,added,2015��8��29��,�Ƿ���Ҫ����̣�Ĭ��Ϊtrue
		bool m_bShowKeyboard;
		//Editʧȥ���������ڼ�¼Edit����λ��
		DWORD m_dwSel;

	
	};
		 
}





#endif //__UITRANS_EDIT_H__