// MediaPlay.h : main header file for the MEDIAPLAY application
//

#if !defined(AFX_MEDIAPLAY_H__0E967902_DAF4_4D37_8E3A_BEDEC7ECE4B1__INCLUDED_)
#define AFX_MEDIAPLAY_H__0E967902_DAF4_4D37_8E3A_BEDEC7ECE4B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMediaPlayApp:
// See MediaPlay.cpp for the implementation of this class
//

class CMediaPlayApp : public CWinApp
{
public:
	CMediaPlayApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMediaPlayApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMediaPlayApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDIAPLAY_H__0E967902_DAF4_4D37_8E3A_BEDEC7ECE4B1__INCLUDED_)
