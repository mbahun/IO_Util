#pragma once
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
#include "resource.h"


class IoUtilApp : public CWinApp{
	public:
		IoUtilApp();
		virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


extern IoUtilApp theApp;