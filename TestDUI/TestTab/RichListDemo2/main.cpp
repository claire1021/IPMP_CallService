#include "stdafx.h"
#include "MainDlg.h"
#include "Resource.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	/*
	在duilib中每个窗口均要定义一个CPaintManagerUI成员对象用来管理整个窗口的绘制,
	duilib中的窗口均继承自CWindowWnd类，在CWindowWnd类中有虚函数HandleMessage来处理Windows消息。
	如果你的窗口想要响应鼠标点击、编辑框内容改变等消息的话，可以把你的窗口类继承INotify接口，
	这样你的窗口上的一个按钮被点击了，可以在继承自INotify接口的Notify函数中进行处理。
	*/
	CPaintManagerUI::SetInstance(hInstance); //将程序实例与皮肤绘制管理器挂钩
	//设置皮肤库的资源路径，资源有图片、xml文件等
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\testTab\\"));  
	MessageBox(NULL,CPaintManagerUI::GetInstancePath(),NULL,MB_ICONHAND); 

	HRESULT Hr = ::CoInitialize(NULL);//初始化COM库, 为加载COM库提供支持  	
	if( FAILED(Hr) ) return 0; 

	CMainDlg MainDlg;
	MainDlg.Create(NULL, _T("MeetingClient"), UI_WNDSTYLE_DIALOG, 0);
	MainDlg.ShowWindow(true);
	CPaintManagerUI::MessageLoop(); 
	return 0;	
	
}