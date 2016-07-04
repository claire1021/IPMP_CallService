// MediaPlayDlg.h : header file
//
//{{AFX_INCLUDES()
#include "wmpplayer4.h"
//}}AFX_INCLUDES

#if !defined(AFX_MEDIAPLAYDLG_H__B5711620_9DF2_4376_A90E_7B45F31D0A7E__INCLUDED_)
#define AFX_MEDIAPLAYDLG_H__B5711620_9DF2_4376_A90E_7B45F31D0A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMediaPlayDlg dialog

class CMediaPlayDlg : public CDialog
{
// Construction
public:
	CMediaPlayDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMediaPlayDlg)
	enum { IDD = IDD_MEDIAPLAY_DIALOG };
	CWMPPlayer4	m_Ctrplay;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMediaPlayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMediaPlayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnOpen();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strFilePathName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDIAPLAYDLG_H__B5711620_9DF2_4376_A90E_7B45F31D0A7E__INCLUDED_)
