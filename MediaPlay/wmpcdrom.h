#if !defined(AFX_WMPCDROM_H__8DB2DEBF_D08C_40AE_94E1_4D0A0BBE5AAA__INCLUDED_)
#define AFX_WMPCDROM_H__8DB2DEBF_D08C_40AE_94E1_4D0A0BBE5AAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CWMPPlaylist;

/////////////////////////////////////////////////////////////////////////////
// CWMPCdrom wrapper class

class CWMPCdrom : public COleDispatchDriver
{
public:
	CWMPCdrom() {}		// Calls COleDispatchDriver default constructor
	CWMPCdrom(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWMPCdrom(const CWMPCdrom& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetDriveSpecifier();
	CWMPPlaylist GetPlaylist();
	void eject();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WMPCDROM_H__8DB2DEBF_D08C_40AE_94E1_4D0A0BBE5AAA__INCLUDED_)
