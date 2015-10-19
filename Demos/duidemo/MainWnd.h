#pragma once
#include "PopWnd.h"
#include "ShlObj.h"

class CDemoFrame : public WindowImplBase, public CWebBrowserEventHandler, public SkinChangedReceiver
{
public:
	CDemoFrame() 
	{
		m_pPopWnd = NULL;
	}

public:
	void InitWindow() 
	{
		CSkinManager::GetSkinManager()->AddReceiver(this);

		m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
		m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxbtn")));
		m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restorebtn")));
		m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));
		m_pSkinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("skinbtn")));
		CWebBrowserUI* pBrowser1 = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("oneclick_browser1")));
		pBrowser1->SetWebBrowserEventHandler(this);
		CWebBrowserUI* pBrowser2 = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("oneclick_browser2")));
		pBrowser2->SetWebBrowserEventHandler(this);
		pBrowser1->NavigateUrl(_T("http://member.yw26.com/c_register.aspx"));
		pBrowser2->NavigateUrl(_T("https://github.com/duisharp/DuiLib_Ultimate"));

		CContainerUI* pPanel = (CContainerUI*)m_PaintManager.FindControl(_T("split_tab_panel"));
		
		CWebBrowserUI* pBrowser3 = new CWebBrowserUI();
		pPanel->Add(pBrowser3);
		pBrowser3->SetName(_T("browser3"));
		pBrowser3->SetAutoNavigation(true);
		pBrowser3->SetHomePage(_T("ftp://ftp.pku.edu.cn"));
		pBrowser3->SetWebBrowserEventHandler(this);
		//pBrowser3->SetDelayCreate(false);
		//pBrowser3->NavigateUrl(_T("www.baidu.com"));

		CComboUI* pFontSize = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("font_size")));
		if(pFontSize)
		{
			CListLabelElementUI * pElement = new CListLabelElementUI();
			pElement->SetText(_T("���Գ�����"));
			pElement->SetFixedHeight(30);
			pElement->SetFixedWidth(120);
			pFontSize->Add(pElement);
		}

		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("listview")));
		for(int i = 0; i < 20; i++)
		{
			CListTextElementUI* pItem  = new CListTextElementUI();
			pItem->SetFixedHeight(30);
			pList->Add(pItem);
			pItem->SetText(0, _T("����"));
			pItem->SetText(1, _T("1000"));
			pItem->SetText(2, _T("100"));
		}
	}

	virtual BOOL Receive(SkinChangedParam param)
	{
		CControlUI* pRoot = m_PaintManager.FindControl(_T("root"));
		if( pRoot != NULL ) {
			if( param.bColor ) {
				pRoot->SetBkColor(param.bkcolor);
				pRoot->SetBkImage(_T(""));
			}
			else {
				pRoot->SetBkColor(0);
				pRoot->SetBkImage(param.bgimage);
			}
		}
		return TRUE;
	}

	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void)
	{
		return S_OK;
	}

	virtual void NewWindow2(IDispatch **pDisp, VARIANT_BOOL *&Cancel)
	{
		*Cancel = TRUE;

	}

	void NewWindow3(IDispatch** ppDisp,VARIANT_BOOL* Cancel,unsigned long dwFlags,BSTR bstrUrlContext,BSTR bstrUrl)
	{
		*Cancel = VARIANT_TRUE;
	}
public:

	DuiLib::CDuiString GetSkinFolder()
	{
#ifdef _DEBUG
		return _T("skin\\duidemo\\");
#else
		return _T("skin\\");
#endif
	}

	DuiLib::CDuiString GetSkinFile()
	{
		return _T("main.xml");
	}

	UILIB_RESOURCETYPE GetResourceType() const
	{
#ifdef _DEBUG
		return UILIB_FILE;
#else
		return UILIB_ZIPRESOURCE;
#endif
	}

	LPCTSTR GetResourceID() const
	{
		return _T("IDR_ZIPRES");
	}

	DuiLib::CDuiString GetZIPFileName() const
	{
		return _T("skin.zip");
	}


	LPCTSTR GetWindowClassName() const
	{ 
		return _T("MainWnd");
	}

	UINT GetClassStyle() const
	{ 
		return CS_DBLCLKS; 
	}

	void OnFinalMessage(HWND hWnd)
	{ 
		delete this;
	}

	void Notify(TNotifyUI& msg)
	{
		if( msg.sType == _T("showactivex") ) 
		{
			if( msg.pSender->GetName().CompareNoCase(_T("ani_flash")) == 0 ) 
			{
				IShockwaveFlash* pFlash = NULL;
				CActiveXUI* pActiveX = static_cast<CActiveXUI*>(msg.pSender);
				pActiveX->GetControl(__uuidof(IShockwaveFlash), (void**)&pFlash);
				if( pFlash != NULL ) 
				{
					pFlash->put_WMode( _bstr_t(_T("Transparent") ) );
					pFlash->put_Movie( _bstr_t(CPaintManagerUI::GetInstancePath() + _T("\\skin\\duidemo\\waterdrop.swf")) );
					pFlash->DisableLocalSecurity();
					pFlash->put_AllowScriptAccess(L"always");
					BSTR response;
					pFlash->CallFunction(L"<invoke name=\"setButtonText\" returntype=\"xml\"><arguments><string>Click me!</string></arguments></invoke>", &response);
					pFlash->Release();
				}  
			}
		}
		else if( msg.sType == _T("click") )
		{
			if( msg.pSender == m_pCloseBtn ) 
			{
				if(IDYES == MessageBox(m_hWnd, _T("ȷ���˳�duidemo��ʾ����"), _T("Duilib�콢��"), MB_YESNO))
				{
					PostQuitMessage(0);
				}
				return; 
			}
			else if( msg.pSender == m_pMinBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
			else if( msg.pSender == m_pMaxBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
			else if( msg.pSender == m_pRestoreBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
			else if( msg.pSender == m_pSkinBtn ) {
				new CSkinFrame(m_hWnd, m_pSkinBtn);
			}
			// ��ť��Ϣ
			OnLClick(msg.pSender);
		}
		else if(msg.sType==_T("selectchanged"))
		{
			CDuiString name = msg.pSender->GetName();
			CTabLayoutUI* pTabSwitch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_switch")));

			if(name.CompareNoCase(_T("basic_tab")) == 0) pTabSwitch->SelectItem(0);
			if(name.CompareNoCase(_T("rich_tab")) == 0) pTabSwitch->SelectItem(1);
			if(name.CompareNoCase(_T("ani_tab")) == 0) pTabSwitch->SelectItem(2);
			if(name.CompareNoCase(_T("split_tab")) == 0) pTabSwitch->SelectItem(3);
		}
	}
	void OnLClick(CControlUI *pControl)
	{
		CDuiString sName = pControl->GetName();
		if(sName.CompareNoCase(_T("homepage_btn")) == 0)
		{
			ShellExecute(NULL, _T("open"), _T("https://github.com/duisharp"), NULL, NULL, SW_SHOW);
		}
		else if(sName.CompareNoCase(_T("popwnd_btn")) == 0)
		{
			if(m_pPopWnd == NULL)
			{
				m_pPopWnd = new CPopWnd();
			}
			if(!::IsWindow(*m_pPopWnd))
			{
				m_pPopWnd->Create(m_hWnd, _T("͸��������ʾ"), WS_POPUP | WS_VISIBLE, 0L, 0, 0, 800, 572);
			}
			m_pPopWnd->CenterWindow();
			m_pPopWnd->ShowModal();
		}
		else if(sName.CompareNoCase(_T("qqgroup_btn")) == 0)
		{
			TCHAR szPath[MAX_PATH] ={0};
			SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES | CSIDL_FLAG_CREATE, NULL, 0, szPath);
			CDuiString sIEPath;
			sIEPath.Format(_T("%s\\Internet Explorer\\iexplore.exe"), szPath);
			ShellExecute(NULL, _T("open"), sIEPath, _T("http://jq.qq.com/?_wv=1027&k=cDTUzr"), NULL, SW_SHOW);
		}
		else if(sName.CompareNoCase(_T("qq_btn")) == 0)
		{
			TCHAR szPath[MAX_PATH] ={0};
			SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES | CSIDL_FLAG_CREATE, NULL, 0, szPath);
			CDuiString sIEPath;
			sIEPath.Format(_T("%s\\Internet Explorer\\iexplore.exe"), szPath);
			ShellExecute(NULL, _T("open"), sIEPath, _T("tencent://Message/?Uin=656067418&Menu=yes"), NULL, SW_SHOW);
		}
		else if(sName.CompareNoCase(_T("menubtn")) == 0)
		{
			CMenuWnd* pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			
			pMenu->Init(NULL, _T("menu.xml"), point, &m_PaintManager);
			CMenuUI* rootMenu = pMenu->GetMenuUI();
			if (rootMenu != NULL)
			{
				CMenuElementUI* pNew = new CMenuElementUI;
				pNew->SetName(_T("Menu_Dynamic"));
				pNew->SetText(_T("��̬һ���˵�"));
				pNew->SetShowExplandIcon(true);
				pNew->SetIcon(_T("WebSit.png"));
				pNew->SetIconSize(16,16);

				CMenuElementUI* pSubNew = new CMenuElementUI;
				pSubNew->SetText(_T("��̬�����˵�"));
				pSubNew->SetName(_T("Menu_Dynamic"));
				pSubNew->SetIcon(_T("Virus.png"));
				pSubNew->SetIconSize(16,16);
				pNew->Add(pSubNew);
				rootMenu->Add(pNew);

				CMenuElementUI* pNew2 = new CMenuElementUI;
				pNew2->SetName(_T("Menu_Dynamic"));
				pNew2->SetText(_T("��̬һ���˵�2"));
				rootMenu->AddAt(pNew2,2);
			}

			// ��̬��Ӻ��������ò˵��Ĵ�С
			pMenu->ResizeMenu();
		}
	}

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// ��ʱ�����յ�WM_NCDESTROY���յ�wParamΪSC_CLOSE��WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) {
			if( !bZoomed ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �رմ��ڣ��˳�����
		if(uMsg == WM_DESTROY)
		{
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		else if (uMsg == WM_MENUCLICK)
		{
			MenuCmd* pMenuCmd = (MenuCmd*)wParam;
			if(pMenuCmd != NULL)
			{
				BOOL bChecked = pMenuCmd->bChecked;
				CDuiString sMenuName = pMenuCmd->szName;
				CDuiString sUserData = pMenuCmd->szUserData;
				CDuiString sText = pMenuCmd->szText;
				delete pMenuCmd;
				pMenuCmd = NULL;

				if ( sMenuName == _T("qianting")) 
				{
					if (bChecked)
					{
						MessageBox(m_hWnd, _T("��Ԥ����Ǳͧ����"), _T(""), 0);
					} 
					else
					{
						MessageBox(m_hWnd, _T("��ȡ����Ǳͧ����"), NULL, 0);
					}			 
				}
				else
				{
					MessageBox(m_hWnd, sText, NULL, 0);
				}
			}
			bHandled = TRUE;
			return 0;
		}
		bHandled = FALSE;
		return 0;
	}

private:
	CPopWnd* m_pPopWnd;
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pSkinBtn;
	std::map<CDuiString, bool> m_MenuInfos;
};
