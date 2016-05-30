#pragma once
class CMainDlg:public WindowImplBase 
{
public:
	CMainDlg(void);
	~CMainDlg(void);

	virtual CDuiString GetSkinFolder();

	virtual CDuiString GetSkinFile();

	virtual LPCTSTR GetWindowClassName( void ) const;

	virtual void InitWindow();

	virtual void Notify( TNotifyUI& msg );

};

