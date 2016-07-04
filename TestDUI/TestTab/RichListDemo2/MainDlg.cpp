#include"MainDlg.h"

CMainDlg::CMainDlg(void)
{
}


CMainDlg::~CMainDlg(void)
{
}

void CMainDlg::InitWindow()
{	
		CenterWindow();
}
	
LRESULT CMainDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE); 
	styleValue &= ~WS_CAPTION; 
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN); 

	//主窗口类与窗口句柄关联
	m_pm.Init(m_hWnd); 
	CDialogBuilder builder; 
	//加载xml并动态创建界面元素，与布局界面元素，核心函数单独分析
	//注意：CDialogBuilder并不是一个对话框类
	CControlUI* pRoot = builder.Create(_T("MainDlg.xml"), (UINT)0, NULL, &m_pm); 
		
	ASSERT(pRoot && "Failed to parse XML"); 

	if (NULL==pRoot)  //如果找不到皮肤文件则退出			
	{ 
		MessageBox(NULL,TEXT("Cant not Find the skin!"),NULL,MB_ICONHAND); 
		return 0; 
	} 
	m_pm.AttachDialog(pRoot); //附加控件数据到HASH表中
	m_pm.AddNotifier(this); //增加通知处理
	return 0; 
}

LRESULT CMainDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT CMainDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ::PostQuitMessage(0L);
    bHandled = FALSE;
    return 0;
}

//在该函数中主要完成窗口各种消息的分派处理，可以在各分支选项中调用对应的函数来完成相应的操作
LRESULT CMainDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;//返回值
    BOOL bHandled = TRUE;//是否被处理了
    switch( uMsg ) {
        //创建窗口
        case WM_CREATE:   lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        //关闭窗口
		case WM_CLOSE:    lRes = OnClose(uMsg, wParam, lParam, bHandled);  break;
		//销毁窗口
		case WM_DESTROY:    lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        default:
            bHandled = FALSE;
    }
    if( bHandled ) return lRes;
	//没有处理，这传送给窗口绘制管理器处理，Notify函数将会在这里的m_pm.MessageHandler函数中被调用
    //duilib库帮处理的消息及相关的处理函数
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    //都不处理则由CWindowWnd处理即应用层和duilib都不处理的消息交由系统默认处理
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CMainDlg::Notify(TNotifyUI& msg)
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
			COptionUI * p = static_cast<COptionUI *>(builder.Create(_T("option.xml"),0,NULL,&m_pm)); 
			if(NULL == p)
				return;
			CContainerUI * pContainer = static_cast<CContainerUI *>(m_pm.FindControl(_T("main_option_layout")));
			int nCount = pContainer->GetCount();
			static int i = 1;
			CDuiString str1;
			
			str1.Format(_T("新增按钮%d"), i);
			p->SetText(str1);
			pContainer->AddAt(p, nCount - 1);
			CTabLayoutUI * pTab = static_cast<CTabLayoutUI *>(m_pm.FindControl(_T("main_tab")));
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
			CTabLayoutUI * pTab = static_cast<CTabLayoutUI *>(m_pm.FindControl(_T("main_tab")));
			if(NULL != pTab)
			{
				CControlUI * pControl = pTab->FindSubControl(_T("tab_01"));
				if(NULL != pControl)
					pTab->SelectItem(pControl);
			}
		}
		else if(msg.pSender->GetName() == _T("option_2"))
		{
			CTabLayoutUI * pTab = static_cast<CTabLayoutUI *>(m_pm.FindControl(_T("main_tab")));
			if(NULL != pTab)
			{
				CControlUI * pControl = pTab->FindSubControl(_T("tab_02"));
				if(NULL != pControl)
					pTab->SelectItem(pControl);
			}
		}
		else if(msg.pSender->GetName() == _T("option"))
		{
			CTabLayoutUI * pTab = static_cast<CTabLayoutUI *>(m_pm.FindControl(_T("main_tab")));
			pTab->SelectItem(msg.pSender->GetTag());
		}
	}
}	