#include "stdafx.h"
#include "IoUtilApp.h"
#include "IoMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(IoUtilApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

IoUtilApp theApp;

IoUtilApp::IoUtilApp(){
}

BOOL IoUtilApp::InitInstance(){
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	IoMainDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}
