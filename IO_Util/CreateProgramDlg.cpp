#include "stdafx.h"
#include "CreateProgramDlg.h"


IMPLEMENT_DYNAMIC(CreateProgramDlg, CDialog)


CreateProgramDlg::CreateProgramDlg(CWnd* pParent) : CDialog(CreateProgramDlg::IDD, pParent){
	m_playerWindow = NULL; 
	m_exitCmdHeight = 0;
	m_exitCmdWidth = 0;
}

CreateProgramDlg::CreateProgramDlg(CString program) : CDialog(CreateProgramDlg::IDD, NULL){
	m_programToUpdate = program;
	m_playerWindow = NULL;
	m_exitCmdHeight = 0;
	m_exitCmdWidth = 0;
}


CreateProgramDlg::~CreateProgramDlg(){
	delete m_playerWindow;
}


void CreateProgramDlg::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, cmdExit);
	DDX_Control(pDX, ID_PROGRAMNAME, txtProgramName);
	DDX_Control(pDX, ID_CREATEPROGRAM, cmdCreateProgram);
	DDX_Control(pDX, ID_RESETPROGRAM, cmdResetProgram);
	DDX_Control(pDX, ID_SAVEPROGRAM, cmdSaveProgramAs);
	DDX_Control(pDX, ID_CMBOUTPUT, cmbOutput);
	DDX_Control(pDX, ID_STARTDELAY, txtStartDelay);
	DDX_Control(pDX, ID_STARTDURATION, txtStartDuration);
	DDX_Control(pDX, ID_STARTSTATE, cmbStartState);
	DDX_Control(pDX, ID_CREATEOUTPUT, cmdCreateOutput);
	DDX_Control(pDX, ID_INPUTSIGNAL, cmbSignal);
	DDX_Control(pDX, ID_SIGNALDURATION, txtSignalDuration);
	DDX_Control(pDX, ID_DELETESIGNAL, cmdDeleteSignal);
	DDX_Control(pDX, ID_INSERTSIGNALAFTER, cmsInsertSignalAfter);
	DDX_Control(pDX, ID_APPENDSIGNAL, cmdSaveSignal);
	DDX_Control(pDX, ID_INPUTMONITOR, cmbInputMonitor);
	DDX_Control(pDX, ID_TRIGERACTION, cmbStateToTrigger);
	DDX_Control(pDX, ID_CREATEACTION, cmdCreateAction);
	DDX_Control(pDX, ID_GRAPHRESOLUTION, cmbTimeResolution);
	DDX_Control(pDX, ID_PROGRAM, cmbProgramToStart);
	DDX_Control(pDX, ID_LOOP, chkLoop);
}


BEGIN_MESSAGE_MAP(CreateProgramDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(ID_CREATEPROGRAM, &CreateProgramDlg::OnBnClickedCreateprogram)
	ON_BN_CLICKED(ID_CREATEOUTPUT, &CreateProgramDlg::OnBnClickedCreateoutput)
	ON_CBN_SELCHANGE(ID_CMBOUTPUT, &CreateProgramDlg::OnCbnSelchangeCmboutput)
	ON_CBN_SELCHANGE(ID_GRAPHRESOLUTION, &CreateProgramDlg::OnCbnSelchangeGraphresolution)
	ON_BN_CLICKED(ID_APPENDSIGNAL, &CreateProgramDlg::OnBnClickedAppendsignal)
	ON_BN_CLICKED(ID_DELETESIGNAL, &CreateProgramDlg::OnBnClickedDeletesignal)
	ON_BN_CLICKED(ID_INSERTSIGNALAFTER, &CreateProgramDlg::OnBnClickedInsertsignalafter)
	ON_BN_CLICKED(ID_RESETPROGRAM, &CreateProgramDlg::OnBnClickedResetprogram)
	ON_BN_CLICKED(ID_SAVEPROGRAM, &CreateProgramDlg::OnBnClickedSaveprogram)
	ON_EN_SETFOCUS(ID_STARTDELAY, &CreateProgramDlg::OnEnSetfocusStartdelay)
	ON_EN_SETFOCUS(ID_STARTDURATION, &CreateProgramDlg::OnEnSetfocusStartduration)
	ON_EN_SETFOCUS(ID_SIGNALDURATION, &CreateProgramDlg::OnEnSetfocusSignalduration)
	ON_CBN_SELCHANGE(ID_TRIGERACTION, &CreateProgramDlg::OnCbnSelchangeTrigeraction)
	ON_CBN_SELCHANGE(ID_INPUTMONITOR, &CreateProgramDlg::OnCbnSelchangeInputmonitor)
	ON_BN_CLICKED(ID_CREATEACTION, &CreateProgramDlg::OnBnClickedCreateaction)
END_MESSAGE_MAP()


void CreateProgramDlg::OnEnSetfocusStartdelay(){
	SetDefID(ID_CREATEOUTPUT);
}


void CreateProgramDlg::OnEnSetfocusStartduration(){
	SetDefID(ID_CREATEOUTPUT);
}	


void CreateProgramDlg::OnEnSetfocusSignalduration(){
	SetDefID(ID_APPENDSIGNAL);
}


BOOL CreateProgramDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	RECT rect;
	GetClientRect(&rect);

	m_playerWindow = new PlayerWindow();
	m_playerWindow->Create(this, rect.right, 0, 0, 150);

	cmdExit.GetWindowRect(&rect);
	m_exitCmdHeight = rect.bottom - rect.top;
	m_exitCmdWidth = rect.right - rect.left;

	cmbInputMonitor.SetCurSel(0);
	cmbStartState.SetCurSel(0);
	cmbStateToTrigger.SetCurSel(0);
	cmbTimeResolution.SetCurSel(1);

	txtSignalDuration.SetLimitText(6);
	txtStartDelay.SetLimitText(5);
	txtStartDuration.SetLimitText(6);

	txtStartDelay.SetWindowTextW(_T("0"));

	if(m_programToUpdate.GetLength()){
		txtProgramName.SetWindowTextW(m_programToUpdate);
		if(!m_playerWindow->LoadProgramFromFile(m_programToUpdate)){
			MessageBox(_T("Unable to load program..."), _T(""), MB_OK | MB_ICONERROR);
			return TRUE;
		}

		cmdCreateProgram.EnableWindow(0);
		txtProgramName.EnableWindow(0);
		cmdCreateProgram.EnableWindow(0);
		cmdResetProgram.EnableWindow();
		cmdSaveProgramAs.EnableWindow();

		m_playerWindow->DrawProgram(cmbTimeResolution.GetCurSel() ? true : false);

		for (int i = 0; i < 8; i++) {
			m_playerWindow->SetInputSignalState(i, m_playerWindow->GetInputMonitorAction(i) ==
				Program::InputAction_Disabled ? -1 : 0);
		}

		OnCbnSelchangeInputmonitor();
		cmdCreateOutput.EnableWindow();
		cmdSaveSignal.EnableWindow();
		cmsInsertSignalAfter.EnableWindow();
		cmdDeleteSignal.EnableWindow();
		cmdCreateAction.EnableWindow();
		cmbOutput.EnableWindow();
		cmbOutput.SetCurSel(0);
		OnCbnSelchangeCmboutput();
 		txtStartDelay.SetFocus();
		txtStartDelay.SetSel(0, -1);
	}

	return TRUE; 
}


void CreateProgramDlg::OnSize(UINT nType, int cx, int cy){
	if (cx < 200) {
		return;
	}

	CDialog::OnSize(nType, cx, cy);	

	if (cmdExit) {
		cmdExit.MoveWindow(cx - m_exitCmdWidth - 10, cy - m_exitCmdHeight - 16, m_exitCmdWidth, m_exitCmdHeight);
	}

	if (m_playerWindow) {
		m_playerWindow->SetNewWidth(cx);
	}
}


void CreateProgramDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI){
	CDialog::OnGetMinMaxInfo(lpMMI);	
	lpMMI->ptMinTrackSize.x = 660;
	lpMMI->ptMinTrackSize.y = 660;
	lpMMI->ptMaxTrackSize.y = 660;
}


void CreateProgramDlg::SetIntToEditBox(unsigned long value, CEdit *box){
	CString tmp;
	tmp.Format(_T("%i"), value);
	box->SetWindowTextW(tmp);
}


unsigned int CreateProgramDlg::GetIntFromEditBox(CEdit *box){
	CString tmp;
	if (box->GetWindowTextLengthW() <= 0) {
		return 0;
	}

	box->GetWindowTextW(tmp);
	return (unsigned int)_wtol(tmp.GetBuffer());
}


void CreateProgramDlg::OnCbnSelchangeGraphresolution(){
	m_playerWindow->DrawProgram(cmbTimeResolution.GetCurSel() ? true : false);
}


void CreateProgramDlg::OnBnClickedCreateprogram(){
	CString programName;
	CString txt;

	if (txtProgramName.GetWindowTextLengthW() <= 0) {
		return;
	}

	txtProgramName.GetWindowTextW(programName);
	for (int i = 0; i < programName.GetLength(); i++) {
		if (programName[i] != '_' && programName[i] != ' '){
			if (!iswalnum(programName[i])) {
				txt.Format(_T("Program name must contain only alpha numeric characters. Change name and try again!"));
				MessageBox(txt, _T("Program name"), MB_OK | MB_ICONEXCLAMATION);
				return;
			}
		}
	}
	
	if (!m_playerWindow->CreateNewProgram(programName)) {
		return;
	}

	txtProgramName.EnableWindow(0);
	cmdCreateProgram.EnableWindow(0);
	cmdResetProgram.EnableWindow();
	cmdSaveProgramAs.EnableWindow();
	m_playerWindow->DrawProgram(cmbTimeResolution.GetCurSel() ? true : false);
	cmdCreateOutput.EnableWindow();
	cmdSaveSignal.EnableWindow();
	cmsInsertSignalAfter.EnableWindow();
	cmdDeleteSignal.EnableWindow();
	cmdCreateAction.EnableWindow();
	cmbOutput.EnableWindow();
	cmbOutput.SetCurSel(0);
	txtStartDelay.SetFocus();
	txtStartDelay.SetSel(0, -1);
}


void CreateProgramDlg::OnCbnSelchangeCmboutput(){
	unsigned long start, duration;
	bool state;
	CString val;
	unsigned int count = 0;

	if(m_playerWindow->GetStartSignalData(cmbOutput.GetCurSel(), &start, &duration, &state)){
		SetIntToEditBox(start, &txtStartDelay);
		SetIntToEditBox(duration, &txtStartDuration);
		state ? cmbStartState.SetCurSel(0) : cmbStartState.SetCurSel(1);

		count = m_playerWindow->GetCountOfSignals(cmbOutput.GetCurSel()); 
		
		cmbSignal.ResetContent();
		for(unsigned int i=1; i<=count; i++){
			val.Format(_T("%i"), i);
			cmbSignal.AddString(val);
		}

		if (cmbSignal.GetCount() > 0) {
			cmbSignal.SetCurSel(cmbSignal.GetCount() - 1);
		}
	}
	else{
		cmbSignal.ResetContent();
		txtStartDelay.SetWindowTextW(_T("0"));
		txtStartDuration.SetWindowTextW(_T(""));
	}

	txtStartDelay.SetFocus();
	txtStartDelay.SetSel(0, -1);
}


void CreateProgramDlg::OnBnClickedCreateoutput(){
	unsigned long start, duration;

	start = GetIntFromEditBox(&txtStartDelay);
	duration = GetIntFromEditBox(&txtStartDuration);

	if(start % 20 || duration % 20 || duration==0){
		MessageBox(_T("Time values must be rounded to 20ms and duration greater than 0!"), 
			_T("Time"), MB_OK | MB_ICONINFORMATION);

		return;
	}

	m_playerWindow->AddNewSignal(cmbOutput.GetCurSel(), start, duration, 
		cmbStartState.GetCurSel() ? false : true, false);

	m_playerWindow->DrawProgram(cmbTimeResolution.GetCurSel() ? true : false);
	txtSignalDuration.SetFocus();
}

void CreateProgramDlg::OnBnClickedAppendsignal(){
	unsigned long duration;
	unsigned int count;
	CString val;

	duration = GetIntFromEditBox(&txtSignalDuration);

	if(duration % 20 || duration == 0){
		MessageBox(_T("Duration must be rounded to 20ms and greater than 0!"), _T("Time"), 
			MB_OK | MB_ICONINFORMATION);

		txtSignalDuration.SetFocus();
		txtSignalDuration.SetSel(0, -1);
		return;
	}

	if(!m_playerWindow->AppendSignal(cmbOutput.GetCurSel(), duration)){
		MessageBox(_T("Output must be created in order to append signals!"), _T("Signal"), 
			MB_OK | MB_ICONINFORMATION);

		txtStartDelay.SetFocus();
		txtStartDelay.SetSel(0, -1);
		return;
	}
	m_playerWindow->DrawProgram(cmbTimeResolution.GetCurSel() ? true : false);

	count = m_playerWindow->GetCountOfSignals(cmbOutput.GetCurSel()); 
	cmbSignal.ResetContent();

	for(unsigned int i=1; i<=count; i++){
		val.Format(_T("%i"), i);
		cmbSignal.AddString(val);
	}

	if (cmbSignal.GetCount() > 0) {
		cmbSignal.SetCurSel(cmbSignal.GetCount() - 1);
	}

	txtSignalDuration.SetSel(0, -1);
}


void CreateProgramDlg::OnBnClickedDeletesignal(){
	if (cmbSignal.GetCount() <= 0) {
		return;
	}

	unsigned int count=0;
	CString val;

	if(!m_playerWindow->DeleteSignalAtIndex(cmbOutput.GetCurSel(), cmbSignal.GetCurSel())){
		MessageBox(_T("Cannot delete selected signal!"), _T("Signal"), MB_OK | MB_ICONERROR);
		return;
	}

	m_playerWindow->DrawProgram(cmbTimeResolution.GetCurSel() ? true : false);

	count = m_playerWindow->GetCountOfSignals(cmbOutput.GetCurSel());
	cmbSignal.ResetContent();

	for(unsigned int i=1; i<=count; i++){
		val.Format(_T("%i"), i);
		cmbSignal.AddString(val);
	}

	if (cmbSignal.GetCount() > 0) {
		cmbSignal.SetCurSel(cmbSignal.GetCount() - 1);
	}
}


void CreateProgramDlg::OnBnClickedInsertsignalafter(){
	if (cmbSignal.GetCount() <= 0) {
		return;
	}

	unsigned long duration=0;
	unsigned int count=0;
	CString val;

	duration = GetIntFromEditBox(&txtSignalDuration);

	if(duration % 20 || duration == 0){
		MessageBox(_T("Duration must be rounded to 20ms and greater than 0!"), _T("Time"), 
			MB_OK | MB_ICONINFORMATION);
		return;
	}

	if(!m_playerWindow->InsertSingalAtIndex(cmbOutput.GetCurSel(), cmbSignal.GetCurSel(), duration)){
		MessageBox(_T("Cannot insert signal at specified index!"), _T("Signal"), MB_OK | MB_ICONERROR);
		return;
	}

	m_playerWindow->DrawProgram(cmbTimeResolution.GetCurSel() ? true : false);

	count = m_playerWindow->GetCountOfSignals(cmbOutput.GetCurSel());
	cmbSignal.ResetContent();

	for(unsigned int i=1; i<=count; i++){
		val.Format(_T("%i"), i);
		cmbSignal.AddString(val);
	}

	if (cmbSignal.GetCount() > 0) {
		cmbSignal.SetCurSel(cmbSignal.GetCount() - 1);
	}
}


void CreateProgramDlg::OnBnClickedResetprogram(){
	CString programName;	
	txtProgramName.GetWindowTextW(programName);
	m_playerWindow->CreateNewProgram(programName);
	m_playerWindow->DrawProgram(cmbTimeResolution.GetCurSel() ? true : false);
	cmbSignal.ResetContent();
}


void CreateProgramDlg::OnBnClickedSaveprogram(){
	if(!m_playerWindow->SaveProgram()){
		MessageBox(_T("Cannot save program to file..."), _T("Save"), MB_OK | MB_ICONERROR);
		return;
	}
	CDialog::OnOK();
}


void CreateProgramDlg::OnCbnSelchangeTrigeraction(){
	if(cmbStateToTrigger.GetCurSel() == 3){
		cmbProgramToStart.EnableWindow(1);
		chkLoop.EnableWindow(1);
		WIN32_FIND_DATAW wfd;
		HANDLE file = NULL;
		cmbProgramToStart.ResetContent();
		file = FindFirstFileW(_T("programs\\*.iop"), &wfd);

		if (file == INVALID_HANDLE_VALUE) {
			return;
		}

		do{
			if (wcslen(wfd.cFileName) > 4) {
				wfd.cFileName[wcslen(wfd.cFileName) - 4] = 0;
			}

			cmbProgramToStart.InsertString(0, wfd.cFileName);
		}while(FindNextFileW(file, &wfd));

		FindClose(file);

		if (cmbProgramToStart.GetCount() > 0) {
			cmbProgramToStart.SetCurSel(0);
		}
	}
	else{
		chkLoop.EnableWindow(0);
		chkLoop.SetCheck(0);
		cmbProgramToStart.ResetContent();
		cmbProgramToStart.EnableWindow(0);
	}
}


void CreateProgramDlg::OnCbnSelchangeInputmonitor(){
	CString program;
	int loop=0;
	WIN32_FIND_DATAW wfd;
	HANDLE file = NULL;

	Program::tag_InputActions act = 
		m_playerWindow->GetInputMonitorAction(cmbInputMonitor.GetCurSel(), &program, &loop);

	cmbProgramToStart.EnableWindow(0);
	chkLoop.EnableWindow(0);

	switch(act){
		case Program::InputAction_Beep:
			cmbStateToTrigger.SetCurSel(6);
			break;

		case Program::InputAction_Disabled:
			cmbStateToTrigger.SetCurSel(0);
			break;

		case Program::InputAction_Indicate:
			cmbStateToTrigger.SetCurSel(7);
			break;

		case Program::InputAction_PauseProgram:
			cmbStateToTrigger.SetCurSel(1);
			break;

		case Program::InputAction_SetOutputsToOne:
			cmbStateToTrigger.SetCurSel(5);
			break;

		case Program::InputAction_SetOutputsToZero:
			cmbStateToTrigger.SetCurSel(4);
			break;

		case Program::InputAction_StartProgram:
			cmbStateToTrigger.SetCurSel(3);
			cmbProgramToStart.EnableWindow();
			chkLoop.EnableWindow();

			cmbProgramToStart.ResetContent();
			file = FindFirstFileW(_T("programs\\*.iop"), &wfd);

			if (file == INVALID_HANDLE_VALUE) {
				return;
			}
			do{
				if (wcslen(wfd.cFileName) > 4) {
					wfd.cFileName[wcslen(wfd.cFileName) - 4] = 0;
				}
				cmbProgramToStart.InsertString(0, wfd.cFileName);

			}while(FindNextFileW(file, &wfd));

			FindClose(file);

			if (cmbProgramToStart.GetCount() > 0) {
				cmbProgramToStart.SelectString(0, program);
			}

			chkLoop.SetCheck(loop);
			break;

		case Program::InputAction_StopProgram:
			cmbStateToTrigger.SetCurSel(2);
			break;
	}
}


void CreateProgramDlg::OnBnClickedCreateaction(){
	CString program;
	int loop=0;
	Program::tag_InputActions act = Program::InputAction_Disabled;

	switch(cmbStateToTrigger.GetCurSel()){
		case 0:
			act = Program::InputAction_Disabled;
			break;

		case 1:
			act = Program::InputAction_PauseProgram;
			break;

		case 2:
			act = Program::InputAction_StopProgram;
			break;

		case 3:
			act = Program::InputAction_StartProgram;
			cmbProgramToStart.GetWindowTextW(program);
			loop = chkLoop.GetCheck();
			break;

		case 4:
			act = Program::InputAction_SetOutputsToZero;
			break;

		case 5:
			act = Program::InputAction_SetOutputsToOne;
			break;

		case 6:
			act = Program::InputAction_Beep;
			break;

		case 7: 
			act = Program::InputAction_Indicate;
			break;
	}
	
	act == Program::InputAction_StartProgram ? 
		m_playerWindow->AddNewInputMonitor(cmbInputMonitor.GetCurSel(), act, &program, loop) : 
		m_playerWindow->AddNewInputMonitor(cmbInputMonitor.GetCurSel(), act);

	m_playerWindow->SetInputSignalState(cmbInputMonitor.GetCurSel(), 
		act == Program::InputAction_Disabled ? -1 : 0);
}

