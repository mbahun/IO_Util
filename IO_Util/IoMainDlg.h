#pragma once
#include "afxwin.h"
#include "PlayerWindow.h"
#include "IODevice.h"
#include "SerialPort.h"

class IoMainDlg : public CDialog{
	private:
		enum DeviceType { IO_DEVICE=0, PC_BEEPER=1 };
		PlayerWindow* m_playerWindow;
		IIODevice* m_ioDevice;
		SerialPort* m_protocol;
		DeviceType m_selectedDevice = IO_DEVICE;

	public:
		IoMainDlg(CWnd* pParent = NULL);
		virtual ~IoMainDlg();
		enum { IDD = IDD_MAIN };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		virtual BOOL OnInitDialog();
		HICON m_hIcon;

	private:
		void FillProgramList();
		int m_exitCmdHeight;
		int m_exitCmdWidth;
		CFont m_titleFont;

	private:
		CListCtrl lstPrograms;
		CComboBox cmbTimeResolution;
		CButton cmdExit;
		CButton chkSimulateHardware;
		CEdit txtLoop;
		CComboBox cmbComPort;
		CComboBox cmbBaudRate;
		CButton cmdOpenSerial;
		CComboBox cmbAvailableHardware;
		CStatic lblTitle;

		virtual void OnCancel();
		virtual void OnOK();	
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnNMDblclkProgstoload(NMHDR* pNMHDR, LRESULT *pResult);
		afx_msg void OnBnClickedLoadprogram();
		afx_msg void OnCbnSelchangeGraphresolution();
		afx_msg void OnBnClickedCancel();
		afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
		afx_msg void OnBnClickedPlay();
		afx_msg void OnBnClickedStop();
		afx_msg void OnBnClickedPause();
		afx_msg void OnBnClickedResume();
		afx_msg void OnBnClickedOpenserialport();
		afx_msg void OnBnClickedDeleteprogram();
		afx_msg void OnBnClickedCreatenew();
		afx_msg void OnBnClickedUpdateprogram();
		afx_msg void OnCbnDropdownComport();
		afx_msg void OnBnClickedLoadinplayer();
		afx_msg void OnCbnSelchangeAvailablehardware();
		afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};
