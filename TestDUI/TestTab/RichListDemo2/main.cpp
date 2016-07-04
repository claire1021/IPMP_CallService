#include "stdafx.h"
#include "MainDlg.h"
#include "Resource.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	/*
	��duilib��ÿ�����ھ�Ҫ����һ��CPaintManagerUI��Ա�������������������ڵĻ���,
	duilib�еĴ��ھ��̳���CWindowWnd�࣬��CWindowWnd�������麯��HandleMessage������Windows��Ϣ��
	�����Ĵ�����Ҫ��Ӧ��������༭�����ݸı����Ϣ�Ļ������԰���Ĵ�����̳�INotify�ӿڣ�
	������Ĵ����ϵ�һ����ť������ˣ������ڼ̳���INotify�ӿڵ�Notify�����н��д���
	*/
	CPaintManagerUI::SetInstance(hInstance); //������ʵ����Ƥ�����ƹ������ҹ�
	//����Ƥ�������Դ·������Դ��ͼƬ��xml�ļ���
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\testTab\\"));  
	MessageBox(NULL,CPaintManagerUI::GetInstancePath(),NULL,MB_ICONHAND); 

	HRESULT Hr = ::CoInitialize(NULL);//��ʼ��COM��, Ϊ����COM���ṩ֧��  	
	if( FAILED(Hr) ) return 0; 

	CMainDlg MainDlg;
	MainDlg.Create(NULL, _T("MeetingClient"), UI_WNDSTYLE_DIALOG, 0);
	MainDlg.ShowWindow(true);
	CPaintManagerUI::MessageLoop(); 
	return 0;	
	
}