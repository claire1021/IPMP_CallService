#include "stdafx.h"
#include "MainDlg.h"


CMainDlg::CMainDlg(void)
{
}


CMainDlg::~CMainDlg(void)
{
}

//*******************************************************************
// ������:    	GetSkinFolder
// �ӿ�����:    public 
// ��������:  	��Դ�ļ���		
// ����ֵ:   	DuiLib::CDuiString		
//*******************************************************************
DuiLib::CDuiString CMainDlg::GetSkinFolder()
{
	//throw std::exception("The method or operation is not implemented.");
		return _T(".\\skin");
}

//*******************************************************************
// ������:    	GetSkinFile
// �ӿ�����:    public 
// ��������:  	��Դxml�ļ���		
// ����ֵ:   	DuiLib::CDuiString		
//*******************************************************************
DuiLib::CDuiString CMainDlg::GetSkinFile()
{
	//throw std::exception("The method or operation is not implemented.");
	return _T("MainDlg.xml");
}

//*******************************************************************
// ������:    	GetWindowClassName
// �ӿ�����:    public 
// ��������:  	��������		
// ����: 		void
// ����ֵ:   	LPCTSTR		
//*******************************************************************
LPCTSTR CMainDlg::GetWindowClassName( void ) const
{
	//throw std::exception("The method or operation is not implemented.");
	return _T("DuiTabTest");
}

//*******************************************************************
// ������:    	InitWindow
// �ӿ�����:    public 
// ��������:  	���ڳ�ʼ�����		
// ����ֵ:   	void		
//*******************************************************************
void CMainDlg::InitWindow()
{
	//throw std::exception("The method or operation is not implemented.");
	//ʹ���ھ���
	CenterWindow();
}

//*******************************************************************
// ������:    	Notify
// �ӿ�����:    public 
// ��������:  	��Ϣ��Ӧ����		
// ����: 		TNotifyUI & msg
// ����ֵ:   	void		
//*******************************************************************
void CMainDlg::Notify( TNotifyUI& msg )
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
			COptionUI * p = static_cast<COptionUI *>(builder.Create(_T("option.xml"),0,NULL,&m_PaintManager)); 
			if(NULL == p)
				return;
			CContainerUI * pContainer = static_cast<CContainerUI *>(m_PaintManager.FindControl(_T("main_option_layout")));
			int nCount = pContainer->GetCount();
			static int i = 1;
			CDuiString str1;
			
			str1.Format(_T("������ť%d"), i);
			p->SetText(str1);
			pContainer->AddAt(p, nCount - 1);
			CTabLayoutUI * pTab = static_cast<CTabLayoutUI *>(m_PaintManager.FindControl(_T("main_tab")));
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