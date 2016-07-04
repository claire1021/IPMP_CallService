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

	//���������봰�ھ������
	m_pm.Init(m_hWnd); 
	CDialogBuilder builder; 
	//����xml����̬��������Ԫ�أ��벼�ֽ���Ԫ�أ����ĺ�����������
	//ע�⣺CDialogBuilder������һ���Ի�����
	CControlUI* pRoot = builder.Create(_T("MainDlg.xml"), (UINT)0, NULL, &m_pm); 
		
	ASSERT(pRoot && "Failed to parse XML"); 

	if (NULL==pRoot)  //����Ҳ���Ƥ���ļ����˳�			
	{ 
		MessageBox(NULL,TEXT("Cant not Find the skin!"),NULL,MB_ICONHAND); 
		return 0; 
	} 
	m_pm.AttachDialog(pRoot); //���ӿؼ����ݵ�HASH����
	m_pm.AddNotifier(this); //����֪ͨ����
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

//�ڸú�������Ҫ��ɴ��ڸ�����Ϣ�ķ��ɴ��������ڸ���֧ѡ���е��ö�Ӧ�ĺ����������Ӧ�Ĳ���
LRESULT CMainDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;//����ֵ
    BOOL bHandled = TRUE;//�Ƿ񱻴�����
    switch( uMsg ) {
        //��������
        case WM_CREATE:   lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        //�رմ���
		case WM_CLOSE:    lRes = OnClose(uMsg, wParam, lParam, bHandled);  break;
		//���ٴ���
		case WM_DESTROY:    lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        default:
            bHandled = FALSE;
    }
    if( bHandled ) return lRes;
	//û�д����⴫�͸����ڻ��ƹ���������Notify���������������m_pm.MessageHandler�����б�����
    //duilib��ﴦ�����Ϣ����صĴ�����
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    //������������CWindowWnd����Ӧ�ò��duilib�����������Ϣ����ϵͳĬ�ϴ���
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CMainDlg::Notify(TNotifyUI& msg)
{
	//throw std::exception("The method or operation is not implemented.");
	//�����¼�
	if(msg.sType == DUI_MSGTYPE_CLICK)
	{ 
		//����رհ�ť
		if(msg.pSender->GetName() == _T("sys_close_btn"))	//�˳�
			Close(0);  
		//�����Ӱ�ť
		else if(msg.pSender->GetName() == _T("add_option"))
		{
			//�½�һ��option��ť
			CDialogBuilder builder;
			COptionUI * p = static_cast<COptionUI *>(builder.Create(_T("option.xml"),0,NULL,&m_pm)); 
			if(NULL == p)
				return;
			CContainerUI * pContainer = static_cast<CContainerUI *>(m_pm.FindControl(_T("main_option_layout")));
			int nCount = pContainer->GetCount();
			static int i = 1;
			CDuiString str1;
			
			str1.Format(_T("������ť%d"), i);
			p->SetText(str1);
			pContainer->AddAt(p, nCount - 1);
			CTabLayoutUI * pTab = static_cast<CTabLayoutUI *>(m_pm.FindControl(_T("main_tab")));
			if(NULL != pTab)
			{
				CDuiString str2;
				str2.Format(_T("��������%d"), i);
				//����һ�����棬����������ϱ�optionһ��ʹ��xml����
				CButtonUI * pButton = new CButtonUI;
				pButton->SetTextColor(0xffffffff);
				pButton->SetText(str2);
				pTab->Add(pButton);
				p->SetTag(pTab->GetCount() - 1);
			}
			 i++;
		}
	}
	//�л�ѡ���ťʱ���ݲ�ͬ�İ�ť��ѡ�в�ͬ�Ľ���
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