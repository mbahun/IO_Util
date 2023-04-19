#include "stdafx.h"
#include "IoUtilApp.h"
#include "IoMainDlg.h"
#include "CreateProgramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(IoMainDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_CANCEL, &IoMainDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, ID_PROGSTOLOAD, &IoMainDlg::OnNMDblclkProgstoload)
	ON_BN_CLICKED(ID_LOADPROGRAM, &IoMainDlg::OnBnClickedLoadprogram)
	ON_CBN_SELCHANGE(ID_GRAPHRESOLUTION, &IoMainDlg::OnCbnSelchangeGraphresolution)
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(ID_PLAY, &IoMainDlg::OnBnClickedPlay)
	ON_BN_CLICKED(ID_STOP, &IoMainDlg::OnBnClickedStop)
	ON_BN_CLICKED(ID_PAUSE, &IoMainDlg::OnBnClickedPause)
	ON_BN_CLICKED(ID_RESUME, &IoMainDlg::OnBnClickedResume)
	ON_BN_CLICKED(ID_OPENSERIALPORT, &IoMainDlg::OnBnClickedOpenserialport)
	ON_BN_CLICKED(ID_DELETEPROGRAM, &IoMainDlg::OnBnClickedDeleteprogram)
	ON_BN_CLICKED(ID_CREATENEW, &IoMainDlg::OnBnClickedCreatenew)
	ON_BN_CLICKED(ID_UPDATEPROGRAM, &IoMainDlg::OnBnClickedUpdateprogram)
	ON_CBN_DROPDOWN(ID_COMPORT, &IoMainDlg::OnCbnDropdownComport)
	ON_BN_CLICKED(ID_SIMULATEHW, &IoMainDlg::OnBnClickedLoadinplayer)
	ON_CBN_SELCHANGE(ID_AVAILABLEHARDWARE, &IoMainDlg::OnCbnSelchangeAvailablehardware)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


IoMainDlg::IoMainDlg(CWnd* pParent) : CDialog(IoMainDlg::IDD, pParent){
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_exitCmdHeight = 0;
	m_exitCmdWidth = 0;

	m_playerWindow = new PlayerWindow();
	m_protocol = new  SerialPort();
	m_ioDevice = new IODevice(m_protocol);
}


IoMainDlg::~IoMainDlg(){
	delete m_playerWindow;
	delete m_ioDevice;
	delete m_protocol;
}


void IoMainDlg::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_PROGSTOLOAD, lstPrograms);
	DDX_Control(pDX, ID_GRAPHRESOLUTION, cmbTimeResolution);
	DDX_Control(pDX, ID_CANCEL, cmdExit);
	DDX_Control(pDX, ID_SIMULATEHW, chkSimulateHardware);
	DDX_Control(pDX, IDC_EDIT2, txtLoop);
	DDX_Control(pDX, ID_COMPORT, cmbComPort);
	DDX_Control(pDX, ID_BAUDRATE, cmbBaudRate);
	DDX_Control(pDX, ID_OPENSERIALPORT, cmdOpenSerial);
	DDX_Control(pDX, ID_AVAILABLEHARDWARE, cmbAvailableHardware);
	DDX_Control(pDX, ID_TITLE, lblTitle);
}


BOOL IoMainDlg::OnInitDialog(){
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_titleFont.CreateFontIndirectW(&lf);
	lblTitle.SetFont(&m_titleFont);

	RECT rect;
	GetClientRect(&rect);

	m_playerWindow->Create(this, rect.right, 10, 235, 30);

	cmdExit.GetWindowRect(&rect);
	m_exitCmdHeight = rect.bottom - rect.top;
	m_exitCmdWidth = rect.right - rect.left;

	lstPrograms.InsertColumn(0, _T("Name"),  LVCFMT_LEFT, 90, -1); 
	lstPrograms.InsertColumn(1, _T("Created"),  LVCFMT_LEFT, 110, -1); 
	lstPrograms.SetExtendedStyle(LVS_EX_BORDERSELECT | LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP);
	FillProgramList();

	cmbTimeResolution.SetCurSel(1);
	chkSimulateHardware.SetCheck(1);
	txtLoop.SetWindowTextW(_T("1"));
	txtLoop.SetLimitText(2);
	cmbBaudRate.SetCurSel(4);
	cmbAvailableHardware.SetCurSel(0);	
	cmbAvailableHardware.EnableWindow(0);

	return TRUE;
}


void IoMainDlg::FillProgramList(){
	WIN32_FIND_DATAW wfd;
	SYSTEMTIME sysTime;
	CString strTime = _T("-");
	HANDLE file = NULL;

	lstPrograms.DeleteAllItems();
		
	file = FindFirstFileW(_T("programs\\*.iop"), &wfd);
	if(file == INVALID_HANDLE_VALUE)
		return;

	do{
		if(FileTimeToSystemTime(&wfd.ftLastWriteTime, &sysTime)){
			SystemTimeToTzSpecificLocalTime(NULL, &sysTime, &sysTime);
			CTime time(sysTime);
			strTime = time.Format(_T("%d.%m.%Y. %H:%M"));
		}	

		if (wcslen(wfd.cFileName) > 4) {
			wfd.cFileName[wcslen(wfd.cFileName) - 4] = 0;
		}

		lstPrograms.InsertItem(0, wfd.cFileName);
		lstPrograms.SetItemText(0, 1, strTime);
	}while(FindNextFileW(file, &wfd));

	FindClose(file);
}


HCURSOR IoMainDlg::OnQueryDragIcon(){
	return static_cast<HCURSOR>(m_hIcon);
}


void IoMainDlg::OnPaint(){
	if (IsIconic()){
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else{
		CDialog::OnPaint();
	}
}


void IoMainDlg::OnSize(UINT nType, int cx, int cy){
	if (cx < 200) {
		return;
	}

	CDialog::OnSize(nType, cx, cy);	

	if (cmdExit) {
		cmdExit.MoveWindow(cx - m_exitCmdWidth - 10, cy - m_exitCmdHeight - 13, m_exitCmdWidth, m_exitCmdHeight);
	}

	m_playerWindow->SetNewWidth(cx);
}


void IoMainDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI){
	CDialog::OnGetMinMaxInfo(lpMMI);	
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;
	lpMMI->ptMaxTrackSize.y = 600;
}


void IoMainDlg::OnNMDblclkProgstoload(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if(m_playerWindow->IsNowPlaying() || pNMItemActivate->iItem < 0){
		*pResult = 0;
		return;
	}
	
	if(!m_playerWindow->LoadProgramFromFile(lstPrograms.GetItemText(pNMItemActivate->iItem, 0))){
		AfxMessageBox(_T("Unable to load program..."));
		return;
	}

	for (int i = 0; i < 8; i++) {
		m_playerWindow->SetInputSignalState(
			i, m_playerWindow->GetInputMonitorAction(i) == Program::InputAction_Disabled ? -1 : 0);
	}

	lblTitle.SetWindowTextW(lstPrograms.GetItemText(pNMItemActivate->iItem, 0));
	if (m_playerWindow->GetTotalDuration() < 12000) {
		m_playerWindow->DrawProgram(true);
	}
	else {
		m_playerWindow->DrawProgram(false);
	}
}


void IoMainDlg::OnBnClickedLoadprogram(){
	int pos = (int)lstPrograms.GetFirstSelectedItemPosition();
	if(m_playerWindow->IsNowPlaying() || pos<=0)
		return;

	pos--;
	
	if(!m_playerWindow->LoadProgramFromFile(lstPrograms.GetItemText(pos, 0))){
		AfxMessageBox(_T("Unable to load program..."));
		return;
	}
	lblTitle.SetWindowTextW(lstPrograms.GetItemText(pos, 0));

	for (int i = 0; i < 8; i++) {
		m_playerWindow->SetInputSignalState(
			i, m_playerWindow->GetInputMonitorAction(i) == Program::InputAction_Disabled ? -1 : 0);
	}

	if(m_playerWindow->GetTotalDuration()<12000){
		m_playerWindow->DrawProgram(true);
		cmbTimeResolution.SetCurSel(1);
	}
	else{
		m_playerWindow->DrawProgram(false);
		cmbTimeResolution.SetCurSel(0);
	}
}


void IoMainDlg::OnBnClickedDeleteprogram(){
	int pos = (int)lstPrograms.GetFirstSelectedItemPosition();
	if (m_playerWindow->IsNowPlaying() || pos <= 0) {
		return;
	}

	pos--;

	CString program = lstPrograms.GetItemText(pos, 0);
	CString text;
	text.Format(_T("Are you sure you want to delete program ''%s''?"), program.GetString());
	
	if (MessageBox(text, _T("Delete program"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES) {
		return;
	}
	
	text.Format(_T("Programs\\%s.iop"), program.GetString());
	if(!DeleteFile(text)){
		AfxMessageBox(_T("Cannot delete program..."), MB_OK | MB_ICONERROR);
		return;
	}
	FillProgramList();
}


void IoMainDlg::OnBnClickedCreatenew(){
	if (m_playerWindow->IsNowPlaying()) {
		return;
	}

	CreateProgramDlg dlg;
	dlg.DoModal();
	FillProgramList();
}


void IoMainDlg::OnBnClickedUpdateprogram(){
	int pos = (int)lstPrograms.GetFirstSelectedItemPosition();
	if (m_playerWindow->IsNowPlaying() || pos <= 0) {
		return;
	}

	pos--;

	CString program = lstPrograms.GetItemText(pos, 0);
	CreateProgramDlg dlg(program.GetBuffer());
	dlg.DoModal();

	m_playerWindow->LoadProgramFromFile(program.GetBuffer());
	for (int i = 0; i < 8; i++) {
		m_playerWindow->SetInputSignalState(
			i, m_playerWindow->GetInputMonitorAction(i) == Program::InputAction_Disabled ? -1 : 0);
	}
	m_playerWindow->DrawProgram();
}


void IoMainDlg::OnCbnSelchangeGraphresolution(){
	if (m_playerWindow->IsNowPlaying()) {
		return;
	}

	cmbTimeResolution.GetCurSel() ? m_playerWindow->SetTimeResolution(true) : m_playerWindow->SetTimeResolution(false);
}


void IoMainDlg::OnBnClickedCancel(){
	if (m_playerWindow->IsNowPlaying()) {
		return;
	}
	
	CDialog::OnCancel();		
}


void IoMainDlg::OnBnClickedPlay(){
	if (m_playerWindow->IsNowPlaying() || !m_playerWindow->IsProgramLoaded()) {
		return;
	}

	if(!m_protocol->IsOpened() && !chkSimulateHardware.GetCheck()) {
		AfxMessageBox(_T("Please open serial port before play or use simulation mode!"), MB_OK | MB_ICONASTERISK);
		return;
	}

	unsigned char loop = 0;
	CString strLoop;
	txtLoop.GetWindowTextW(strLoop);
	loop = (unsigned char)_wtoi(strLoop.GetBuffer());

	chkSimulateHardware.GetCheck() ? m_playerWindow->Play(loop) : m_playerWindow->Play(loop, m_ioDevice);
}


void IoMainDlg::OnBnClickedStop(){
	m_playerWindow->Stop();
}


void IoMainDlg::OnBnClickedPause(){	
	m_playerWindow->Pause();
}


void IoMainDlg::OnBnClickedResume(){
	m_playerWindow->Resume();
}


void IoMainDlg::OnBnClickedOpenserialport(){
	if (m_playerWindow->IsNowPlaying()) {
		return;
	}

	unsigned char com=0;
	unsigned long baud=0;
	CString tmp;

	if(m_protocol->IsOpened()){
		m_protocol->Close();
		cmdOpenSerial.SetWindowTextW(_T("Open serial port"));
		cmbComPort.EnableWindow();
	}
	else{
		if(!cmbComPort.GetWindowTextLengthW()){
			AfxMessageBox(_T("Select serial port to open!"), MB_OK | MB_ICONASTERISK);
			return;
		}

		cmbComPort.GetWindowTextW(tmp);
		com = (unsigned char)_wtoi(tmp.GetBuffer());

		cmbBaudRate.GetLBText(cmbBaudRate.GetCurSel(), tmp);
		baud = (unsigned long)_wtoi(tmp.GetBuffer());

		if(!m_protocol->Open(com, baud)) {
			AfxMessageBox(_T("Cannot open selected serial port..."), MB_OK | MB_ICONERROR);
			return;
		}
		cmdOpenSerial.SetWindowTextW(_T("Close port"));
		cmbComPort.EnableWindow(0);
	}
}


void IoMainDlg::OnCbnDropdownComport(){
	if (m_playerWindow->IsNowPlaying()) {
		return;
	}

	unsigned char c=1;
	CString com; 

	cmbComPort.ResetContent();
	while(1){
		c = m_protocol->SearchSerialPorts(c);
		if (c == 0) {
			break;
		}
		com.Format(_T("%i"), c);
		cmbComPort.AddString(com);
		c++;
	}
}


void IoMainDlg::OnBnClickedLoadinplayer(){
	chkSimulateHardware.GetCheck() ? cmbAvailableHardware.EnableWindow(0) : cmbAvailableHardware.EnableWindow(1);
}


void IoMainDlg::OnCbnSelchangeAvailablehardware(){
	if (m_playerWindow->IsNowPlaying()) {
		return;
	}

	delete m_ioDevice;

	switch ((DeviceType)cmbAvailableHardware.GetCurSel()) {
		case IO_DEVICE:
			m_ioDevice = new IODevice(m_protocol);
			break;

		case PC_BEEPER:
			m_ioDevice = new Beeper();
			break;

		default:
			m_ioDevice = new IODevice(m_protocol);

	}
}


void IoMainDlg::OnClose(){
	if (m_playerWindow->IsNowPlaying()) {
		return;
	}

	CDialog::OnCancel();
}


void IoMainDlg::OnCancel(){
}


void IoMainDlg::OnOK(){
}

