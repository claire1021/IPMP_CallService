#include "stdafx.h"
#include "MainDlg.h"


CMainDlg::CMainDlg(void)
{
}


CMainDlg::~CMainDlg(void)
{
}

//*******************************************************************
// 函数名:    	GetSkinFolder
// 接口类型:    public 
// 功能描述:  	资源文件夹		
// 返回值:   	DuiLib::CDuiString		
//*******************************************************************
DuiLib::CDuiString CMainDlg::GetSkinFolder()
{
	//throw std::exception("The method or operation is not implemented.");
		return _T(".\\skin");
}

//*******************************************************************
// 函数名:    	GetSkinFile
// 接口类型:    public 
// 功能描述:  	资源xml文件名		
// 返回值:   	DuiLib::CDuiString		
//*******************************************************************
DuiLib::CDuiString CMainDlg::GetSkinFile()
{
	//throw std::exception("The method or operation is not implemented.");
	return _T("MainDlg.xml");
}

//*******************************************************************
// 函数名:    	GetWindowClassName
// 接口类型:    public 
// 功能描述:  	窗口名称		
// 参数: 		void
// 返回值:   	LPCTSTR		
//*******************************************************************
LPCTSTR CMainDlg::GetWindowClassName( void ) const
{
	//throw std::exception("The method or operation is not implemented.");
	return _T("DuiTabTest");
}

//*******************************************************************
// 函数名:    	InitWindow
// 接口类型:    public 
// 功能描述:  	窗口初始化完成		
// 返回值:   	void		
//*******************************************************************
void CMainDlg::InitWindow()
{
	//throw std::exception("The method or operation is not implemented.");
	//使窗口居中
	CenterWindow();
}

//*******************************************************************
// 函数名:    	Notify
// 接口类型:    public 
// 功能描述:  	消息响应函数		
// 参数: 		TNotifyUI & msg
// 返回值:   	void		
//*******************************************************************
void CMainDlg::Notify( TNotifyUI& msg )
{
	//throw std::exception("The method or operation is not implemented.");
	//单击事件
	if(msg.sType == DUI_MSGTYPE_CLICK)
	{ 
		//点击关闭按钮
		if(msg.pSender->GetName() == _T("sys_close_btn"))	//退出
			Close(0);  
		//点击添加按钮
		else if(msg.pSender->GetName() == _T("add_option"))
		{
			//新建一个option按钮
			CDialogBuilder builder;
			COptionUI * p = static_cast<COptionUI *>(builder.Create(_T("option.xml"),0,NULL,&m_PaintManager)); 
			if(NULL == p)
				return;
			CContainerUI * pContainer = static_cast<CContainerUI *>(m_PaintManager.FindControl(_T("main_option_layout")));
			int nCount = pContainer->GetCount();
			static int i = 1;
			CDuiString str1;
			
			str1.Format(_T("新增按钮%d"), i);
			p->SetText(str1);
			pContainer->AddAt(p, nCount - 1);
			CTabLayoutUI * pTab = static_cast<CTabLayoutUI *>(m_PaintManager.FindControl(_T("main_tab")));
			if(NULL != pTab)
			{
				CDuiString str2;
				str2.Format(_T("新增界面%d"), i);
				//增加一个界面，这里可以像上边option一样使用xml创建
				CButtonUI * pButton = new CButtonUI;
				pButton->SetTextColor(0xffffffff);
				pButton->SetText(str2);
				pTab->Add(pButton);
				p->SetTag(pTab->GetCount() - 1);
			}
			 i++;
		}
	}
	//切换选项卡按钮时根据不同的按钮，选中不同的界面
	else if(msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if(msg.pSender->GetName() == _T("option_1"))
		{
			CTabLayoutUI * pTab = static_cast<CTabLayoutUI *>(m_PaintManager.FindControl(_T("main_tab")));
			if(NULL != pTab)
			{
				CControlUI * pControl = pTab->FindSubControl(_T("tab_01"));
				if(NULL != pControl)
					pTab->SelectItem(pControl);
			}
		}
		else if(msg.pSender->GetName() == _T("option_2"))
		{
			CTabLayoutUI * pTab = static_cast<CTabLayoutUI *>(m_PaintManager.FindControl(_T("main_tab")));
			if(NULL != pTab)
			{
				CControlUI * pControl = pTab->FindSubControl(_T("tab_02"));
				if(NULL != pControl)
					pTab->SelectItem(pControl);
			}
		}
		else if(msg.pSender->GetName() == _T("option"))
		{
			CTabLayoutUI * pTab = static_cast<CTabLayoutUI *>(m_PaintManager.FindControl(_T("main_tab")));
			pTab->SelectItem(msg.pSender->GetTag());
		}
	}
}