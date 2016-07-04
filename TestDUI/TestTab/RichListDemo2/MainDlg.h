#pragma once
#include"StdAfx.h"
class CMainDlg:public CWindowWnd,public INotifyUI //应用程序窗口类、通知类
{
public:
	CMainDlg(void);
	~CMainDlg(void);
	CPaintManagerUI m_pm;//每个窗口都要有一个CPainManagerUI管理

	LPCTSTR GetWindowClassName() const { return _T("ScanMainForm"); }
    UINT GetClassStyle() const{  return CS_DBLCLKS; }	
	void OnFinalMessage(HWND /*hWnd*/) {delete this;}

	void InitWindow();

	void Notify( TNotifyUI& msg );
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnPrepare(TNotifyUI& msg);


	 //主要把皮肤文件名“AdderCalculater.xml”添加上
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

