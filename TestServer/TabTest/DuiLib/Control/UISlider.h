#ifndef __UISLIDER_H__
#define __UISLIDER_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CSliderUI : public CProgressUI
	{
	public:
		CSliderUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetEnabled(bool bEnable = true);

		int GetChangeStep();
		void SetChangeStep(int step);
		void SetThumbSize(SIZE szXY);
		RECT GetThumbRect() const;
		LPCTSTR GetThumbImage() const;
		void SetThumbImage(LPCTSTR pStrImage);
		LPCTSTR GetThumbHotImage() const;
		void SetThumbHotImage(LPCTSTR pStrImage);
		LPCTSTR GetThumbPushedImage() const;
		void SetThumbPushedImage(LPCTSTR pStrImage);

		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void PaintStatusImage(HDC hDC);

		//设置特殊滑块，滑块中间起点
		void SetSpecialSlider(bool bSpecial);
	protected:
		SIZE m_szThumb;
		UINT m_uButtonState;
		int m_nStep;

		TDrawInfo m_diThumb;
		TDrawInfo m_diThumbHot;
		TDrawInfo m_diThumbPushed;
		//slider定制(轨迹从滑块中间开始画)
		bool	m_bSpecialThumb;
	};
}

#endif // __UISLIDER_H__