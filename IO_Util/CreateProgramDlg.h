#pragma once
#include "resource.h"
#include "PlayerWindow.h"
#include "afxwin.h"


class CreateProgramDlg : public CDialog{
	DECLARE_DYNAMIC(CreateProgramDlg)
	
	public:
		CreateProgramDlg(CWnd* pParent = NULL);
		CreateProgramDlg(CString program);

		virtual ~CreateProgramDlg();
		enum { IDD = IDD_NEWPROGRAM };

	private:
		PlayerWindow *m_playerWindow;
		CString m_programToUpdate;
		int m_exitCmdHeight;
		int m_exitCmdWidth;

	private:
		void SetIntToEditBox(unsigned long value, CEdit *box);
		unsigned int GetIntFromEditBox(CEdit *box);

	private:
		virtual void DoDataExchange(CDataExchange* pDX);
		virtual BOOL OnInitDialog();

		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
		afx_msg void OnBnClickedCreateprogram();
		afx_msg void OnBnClickedCreateoutput();
		afx_msg void OnCbnSelchangeCmboutput();
		afx_msg void OnCbnSelchangeGraphresolution();
		afx_msg void OnBnClickedAppendsignal();
		afx_msg void OnBnClickedDeletesignal();
		afx_msg void OnBnClickedInsertsignalafter();
		afx_msg void OnBnClickedResetprogram();
		afx_msg void OnBnClickedSaveprogram();
		afx_msg void OnEnSetfocusStartdelay();
		afx_msg void OnEnSetfocusStartduration();
		afx_msg void OnEnSetfocusSignalduration();		
		afx_msg void OnCbnSelchangeTrigeraction();
		afx_msg void OnCbnSelchangeInputmonitor();
		afx_msg void OnBnClickedCreateaction();
		
		CComboBox cmbTimeResolution;
		CEdit txtProgramName;
		CButton cmdCreateProgram;
		CButton cmdResetProgram;
		CButton cmdSaveProgramAs;

		CComboBox cmbOutput;
		CEdit txtStartDelay;
		CEdit txtStartDuration;
		CComboBox cmbStartState;
		CButton cmdCreateOutput;

		CComboBox cmbSignal;
		CEdit txtSignalDuration;
		CButton cmdDeleteSignal;
		CButton cmsInsertSignalAfter;
		CButton cmdSaveSignal;

		CComboBox cmbInputMonitor;
		CComboBox cmbStateToTrigger;
		CButton cmdCreateAction;
		CComboBox cmbProgramToStart;
		CButton chkLoop;

		CButton cmdExit;

	DECLARE_MESSAGE_MAP()
};
