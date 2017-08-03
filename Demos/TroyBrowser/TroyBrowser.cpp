#include "StdAfx.h"
#include "resource.h"
#include "MainWnd.h"

void InitResource()
{	
	// ��Դ����
#ifdef _DEBUG
	//CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);

	CPaintManagerUI::SetResourceType(UILIB_FILE);
#else
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
#endif
	// ��Դ·��
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	// ������Դ
	switch(CPaintManagerUI::GetResourceType())
	{
	case UILIB_FILE:
		{
			strResourcePath += _T("Skin\\TroyBrowser\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			// ������Դ������
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_RESOURCE:
		{
			strResourcePath += _T("Skin\\TroyBrowser\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			// ������Դ������
			CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
			break;
		}
	case UILIB_ZIP:
		{
			strResourcePath += _T("Skin\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			CPaintManagerUI::SetResourceZip(_T("TroyBrowser.zip"), true);
			// ������Դ������
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_ZIPRESOURCE:
		{
			strResourcePath += _T("Skin\\TroyBrowser\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

			HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
			if( hResource != NULL ) {
				DWORD dwSize = 0;
				HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
				if( hGlobal != NULL ) {
					dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
					if( dwSize > 0 ) {
						CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
						// ������Դ������
						CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
					}
				}
				::FreeResource(hResource);
			}
		}
		break;
	}
}
void WINAPI WriteWebBrowserRegKey(LPCTSTR lpKey,DWORD dwValue)
{
	HKEY hk;
	//CDuiString str = L"Software\\WOW6432Node\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\";
	//str += lpKey;
	//RegCreateKey(HKEY_CURRENT_USER,str.GetData(),&hk);
	//RegSetValueEx(hk,L"TroyBrowser.exe",NULL,REG_DWORD,(const byte*)&dwValue,4);
	//RegCloseKey(hk);

	CDuiString str1 = L"Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\";
	str1 += lpKey;
	RegCreateKey(HKEY_CURRENT_USER,str1.GetData(),&hk);
	RegSetValueEx(hk,L"TroyBrowser_d.exe",NULL,REG_DWORD,(const byte*)&dwValue,4);
	RegCloseKey(hk);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	WriteWebBrowserRegKey(L"FEATURE_BLOCK_LMZ_IMG",1);
	WriteWebBrowserRegKey(L"FEATURE_BLOCK_LMZ_OBJECT",1);
	WriteWebBrowserRegKey(L"FEATURE_BLOCK_LMZ_SCRIPT",1);
	WriteWebBrowserRegKey(L"FEATURE_Cross_Domain_Redirect_Mitigation",1);
	WriteWebBrowserRegKey(L"FEATURE_ENABLE_SCRIPT_PASTE_URLACTION_IF_PROMPT",1);
	WriteWebBrowserRegKey(L"FEATURE_LOCALMACHINE_LOCKDOWN",1);

	WriteWebBrowserRegKey(L"FEATURE_INTERNET_SHELL_FOLDERS",0);
	WriteWebBrowserRegKey(L"FEATURE_BROWSER_EMULATION",11000);
	WriteWebBrowserRegKey(L"FEATURE_ZONE_ELEVATION",1);
	WriteWebBrowserRegKey(L"FEATURE_SCRIPTURL_MITIGATION",1);
	WriteWebBrowserRegKey(L"FEATURE_BLOCK_LMZ_SCRIPT",1);
	WriteWebBrowserRegKey(L"FEATURE_GPU_RENDERING",1);

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	HRESULT hRes = ::OleInitialize(NULL);
	// ��ʼ��UI������
	CPaintManagerUI::SetInstance(hInstance);
	// ��ʼ����Դ
	InitResource();

	::SetCurrentDirectory(CPaintManagerUI::GetInstancePath());
#ifndef _DEBUG
	CPaintManagerUI::LoadPlugin(_T("TroyControls.dll"));
#else
	CPaintManagerUI::LoadPlugin(_T("TroyControls_d.dll"));
#endif
	CMainWnd* pFrame = new CMainWnd();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("TroyBrowser"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 990,690);
	pFrame->CenterWindow();
	pFrame->ShowModal();

	CPaintManagerUI::MessageLoop();
	OleUninitialize();
	::CoUninitialize();
	return 0;
}