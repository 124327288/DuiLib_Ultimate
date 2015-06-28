// ADMonSetup.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ADMonSetup.h"
#include "MainWnd.h"

//#include <GdiPlus.h>
//#pragma comment( lib, "GdiPlus.lib" )
//using namespace Gdiplus;

//----------------------------------------------------------------------------------------------------------------------------------------

//������ڴ����øú��������ڴ�й¶���
//�����˳�ǰ����_CrtDumpMemoryLeaks();�����ɲ鿴�ڴ�й¶��ID�ţ���ID�ſ���ΪBreakpoint�Ĳ���������ʱ��������Ӧλ���Զ��ϵ�
inline  void  EnableMemLeakCheck(int Breakpoint = 0){
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)  |  _CRTDBG_LEAK_CHECK_DF);
	if(Breakpoint)
		_CrtSetBreakAlloc ( 1912 );
}

#ifdef _DEBUG
#define  new   new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#endif 
#define _CRT_SECURE_NO_DEPRECATE

//----------------------------------------------------------------------------------------------------------------------------------------

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	EnableMemLeakCheck();
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\ADMonSetup"));

	//GdiplusStartupInput   gdiplusStartupInput;
	//ULONG_PTR             gdiplusToken;
	//GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	if( ::LoadLibrary(_T("d3d9.dll")) == NULL ) 
		::MessageBox(NULL, _T("���� d3d9.dll ʧ�ܣ�һЩ��Ч�����޷���ʾ��"), _T("��Ϣ��ʾ"),MB_OK|MB_ICONWARNING);

	CMainWnd* pFrame = new CMainWnd();
	if(pFrame == NULL)
		return 0;

	pFrame->Create(NULL,_T("��Ƶ�����˴�ʦ��װ��"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 900, 600);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	//GdiplusShutdown(gdiplusToken);

	_CrtDumpMemoryLeaks();
	return 0;
}
