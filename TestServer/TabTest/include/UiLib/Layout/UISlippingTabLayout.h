#ifndef __UISLIPPINGTABLAYOUT_H__
#define __UISLIPPINGTABLAYOUT_H__

#pragma once 

using namespace std;
namespace DuiLib
{

	class UILIB_API CSlippingTabLayoutUI : public CTabLayoutUI//, public CUIAnimation
	{
	public:
		CSlippingTabLayoutUI();
		~CSlippingTabLayoutUI();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool SelectItem( int iIndex );
		bool SelectItem(CControlUI * pControl);
		void AnimationSwitch();  
		 
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame); 

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		static DWORD WINAPI SlippingProc ( LPVOID pParam );

	protected:
		bool m_bIsVerticalDirection;
		int m_nPositiveDirection;
		RECT m_rcCurPos;
		RECT m_rcItemOld;
		CControlUI* m_pCurrentControl;
		bool m_bControlVisibleFlag;
		enum
		{  
			TAB_ANIMATION_ELLAPSE = 10,
			TAB_ANIMATION_FRAME_COUNT = 30,
		};
	};
}
#endif // __UISLIPPINGTABLAYOUT_H__