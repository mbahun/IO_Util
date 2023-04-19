#include "stdafx.h"
#include "IoUtilApp.h"
#include "PlayerWindow.h"

IMPLEMENT_DYNAMIC(PlayerWindow, CDialog)

PlayerWindow::PlayerWindow(): CDialog(PlayerWindow::IDD, 0){
	m_height = 350;
	m_inputSpaceHeight = 70;
	m_currentScrollPos = 0;
	m_ioDevice = NULL;

	m_leftMargin = 0;
	m_rightMargin = 0;
	m_topMargin	= 0;
	m_width	= 0;

	m_ioThreadData.ioDevice = NULL;
	m_ioThreadData.isRunning = 0;
	m_ioThreadData.isThreadLooping = 0;
	m_ioThreadData.program = NULL;
	m_ioThreadData.time = 0;
}


PlayerWindow::~PlayerWindow(){
}


void PlayerWindow::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR1, cntScroll);
	DDX_Control(pDX, ID_PROGRAM_PLAYER, m_programPlayer);
	DDX_Control(pDX, IDC_EDIT1, txtTime);
	DDX_Control(pDX, ID_DIS1, picDis1);
	DDX_Control(pDX, ID_DIS2, picDis2);
	DDX_Control(pDX, ID_DIS3, picDis3);
	DDX_Control(pDX, ID_DIS4, picDis4);
	DDX_Control(pDX, ID_DIS5, picDis5);
	DDX_Control(pDX, ID_DIS6, picDis6);
	DDX_Control(pDX, ID_DIS7, picDis7);
	DDX_Control(pDX, ID_DIS8, picDis8);
	DDX_Control(pDX, ID_ACTIVE1, picAct1);
	DDX_Control(pDX, ID_ACTIVE2, picAct2);
	DDX_Control(pDX, ID_ACTIVE3, picAct3);
	DDX_Control(pDX, ID_ACTIVE4, picAct4);
	DDX_Control(pDX, ID_ACTIVE5, picAct5);
	DDX_Control(pDX, ID_ACTIVE6, picAct6);
	DDX_Control(pDX, ID_ACTIVE7, picAct7);
	DDX_Control(pDX, ID_ACTIVE8, picAct8);
	DDX_Control(pDX, ID_NONACT1, picNoAct1);
	DDX_Control(pDX, ID_NONACT2, picNoAct2);
	DDX_Control(pDX, ID_NONACT3, picNoAct3);
	DDX_Control(pDX, ID_NONACT4, picNoAct4);
	DDX_Control(pDX, ID_NONACT5, picNoAct5);
	DDX_Control(pDX, ID_NONACT6, picNoAct6);
	DDX_Control(pDX, ID_NONACT7, picNoAct7);
	DDX_Control(pDX, ID_NONACT8, picNoAct8);
}


BEGIN_MESSAGE_MAP(PlayerWindow, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_USER+1, OnPlayerStop)
	ON_MESSAGE(WM_USER+2, OnTimeChange)
	ON_MESSAGE(WM_USER+3, OnInput)
	ON_MESSAGE(WM_USER+4, OnResetInput)
END_MESSAGE_MAP()


void PlayerWindow::OnPaint(){
	CPaintDC dc(this);
}


BOOL PlayerWindow::OnInitDialog(){
	CDialog::OnInitDialog();
	txtTime.SetWindowTextW(_T(""));
	return TRUE; 
}


void PlayerWindow::ArrangeInputPics() {
	int startX = 18;

	if (!picDis1.GetSafeHwnd()) {
		return;
	}

	picDis1.MoveWindow(startX, m_height + 45, 16, 16);	
	picDis2.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picDis3.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picDis4.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picDis5.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picDis6.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picDis7.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picDis8.MoveWindow(startX+=48, m_height + 45, 16, 16);	

	startX=18;
	picAct1.MoveWindow(startX, m_height + 45, 16, 16);	
	picAct2.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picAct3.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picAct4.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picAct5.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picAct6.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picAct7.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picAct8.MoveWindow(startX+=48, m_height + 45, 16, 16);	

	startX=18;
	picNoAct1.MoveWindow(startX, m_height + 45, 16, 16);	
	picNoAct2.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picNoAct3.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picNoAct4.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picNoAct5.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picNoAct6.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picNoAct7.MoveWindow(startX+=48, m_height + 45, 16, 16);	
	picNoAct8.MoveWindow(startX+=48, m_height + 45, 16, 16);	
}


void PlayerWindow::OnSize(UINT nType, int cx, int cy){
	CDialog::OnSize(nType, cx, cy);
	m_width = cx;

	if (m_programPlayer.GetSafeHwnd()) {
		m_programPlayer.MoveWindow(0, 0, m_width, m_height);
	}

	if (cntScroll.GetSafeHwnd()) {
		cntScroll.MoveWindow(0, m_height, m_width, 20);
	}

	if (txtTime.GetSafeHwnd()) {
		txtTime.MoveWindow(0, m_height + 21, 150, 20);
	}

	ArrangeInputPics();
}


void PlayerWindow::Create(CWnd *parent, int parentWidth, int rightMargin, int leftMargin, int topMargin){
	if (leftMargin) {
		parentWidth -= leftMargin;
	}

	if (rightMargin) {
		parentWidth -= rightMargin;
	}

	m_leftMargin = leftMargin;
	m_rightMargin = rightMargin;
	m_topMargin = topMargin;

	__super::Create(IDD_PLAYER_WINDOW, parent);
	MoveWindow(m_leftMargin, m_topMargin, parentWidth, m_height + m_inputSpaceHeight);
	ShowWindow(SW_SHOW);
	AdjustScrollBar();
}


void PlayerWindow::SetNewWidth(int parentWidth){
	if (m_hWnd == NULL) {
		return;
	}

	if(m_leftMargin)
		parentWidth -= m_leftMargin;
	if(m_rightMargin)
		parentWidth -= m_rightMargin;

	MoveWindow(m_leftMargin, m_topMargin, parentWidth, m_height + m_inputSpaceHeight);

	if(cntScroll.IsWindowEnabled())
		AdjustScrollBar();
}


void PlayerWindow::SetInputSignalsStates(char s1, char s2, char s3, char s4, char s5, char s6, char s7, char s8){
	//-1= disabled, 0=not active, 1=active, >=3as is
	if(s1==-1){picDis1.ShowWindow(SW_NORMAL); picAct1.ShowWindow(SW_HIDE); picNoAct1.ShowWindow(SW_HIDE);}
	else if(s1==0){picDis1.ShowWindow(SW_HIDE); picAct1.ShowWindow(SW_HIDE); picNoAct1.ShowWindow(SW_NORMAL);}
	else if(s1==1){picDis1.ShowWindow(SW_HIDE); picAct1.ShowWindow(SW_NORMAL); picNoAct1.ShowWindow(SW_HIDE);}

	if(s2==-1){picDis2.ShowWindow(SW_NORMAL); picAct2.ShowWindow(SW_HIDE); picNoAct2.ShowWindow(SW_HIDE);}
	else if(s2==0){picDis2.ShowWindow(SW_HIDE); picAct2.ShowWindow(SW_HIDE); picNoAct2.ShowWindow(SW_NORMAL);}
	else if(s2==1){picDis2.ShowWindow(SW_HIDE); picAct2.ShowWindow(SW_NORMAL); picNoAct2.ShowWindow(SW_HIDE);}

	if(s3==-1){picDis3.ShowWindow(SW_NORMAL); picAct3.ShowWindow(SW_HIDE); picNoAct3.ShowWindow(SW_HIDE);}
	else if(s3==0){picDis3.ShowWindow(SW_HIDE); picAct3.ShowWindow(SW_HIDE); picNoAct3.ShowWindow(SW_NORMAL);}
	else if(s3==1){picDis3.ShowWindow(SW_HIDE); picAct3.ShowWindow(SW_NORMAL); picNoAct3.ShowWindow(SW_HIDE);}

	if(s4==-1){picDis4.ShowWindow(SW_NORMAL); picAct4.ShowWindow(SW_HIDE); picNoAct4.ShowWindow(SW_HIDE);}
	else if(s4==0){picDis4.ShowWindow(SW_HIDE); picAct4.ShowWindow(SW_HIDE); picNoAct4.ShowWindow(SW_NORMAL);}
	else if(s4==1){picDis4.ShowWindow(SW_HIDE); picAct4.ShowWindow(SW_NORMAL); picNoAct4.ShowWindow(SW_HIDE);}

	if(s5==-1){picDis5.ShowWindow(SW_NORMAL); picAct5.ShowWindow(SW_HIDE); picNoAct5.ShowWindow(SW_HIDE);}
	else if(s5==0){picDis5.ShowWindow(SW_HIDE); picAct5.ShowWindow(SW_HIDE); picNoAct5.ShowWindow(SW_NORMAL);}
	else if(s5==1){picDis5.ShowWindow(SW_HIDE); picAct5.ShowWindow(SW_NORMAL); picNoAct5.ShowWindow(SW_HIDE);}

	if(s6==-1){picDis6.ShowWindow(SW_NORMAL); picAct6.ShowWindow(SW_HIDE); picNoAct6.ShowWindow(SW_HIDE);}
	else if(s6==0){picDis6.ShowWindow(SW_HIDE); picAct6.ShowWindow(SW_HIDE); picNoAct6.ShowWindow(SW_NORMAL);}
	else if(s6==1){picDis6.ShowWindow(SW_HIDE); picAct6.ShowWindow(SW_NORMAL); picNoAct6.ShowWindow(SW_HIDE);}

	if(s7==-1){picDis7.ShowWindow(SW_NORMAL); picAct7.ShowWindow(SW_HIDE); picNoAct7.ShowWindow(SW_HIDE);}
	else if(s7==0){picDis7.ShowWindow(SW_HIDE); picAct7.ShowWindow(SW_HIDE); picNoAct7.ShowWindow(SW_NORMAL);}
	else if(s7==1){picDis7.ShowWindow(SW_HIDE); picAct7.ShowWindow(SW_NORMAL); picNoAct7.ShowWindow(SW_HIDE);}

	if (s8==-1){picDis8.ShowWindow(SW_NORMAL); picAct8.ShowWindow(SW_HIDE); picNoAct8.ShowWindow(SW_HIDE);}
	else if	(s8==0){picDis8.ShowWindow(SW_HIDE); picAct8.ShowWindow(SW_HIDE); picNoAct8.ShowWindow(SW_NORMAL);}
	else if	(s8==1){picDis8.ShowWindow(SW_HIDE); picAct8.ShowWindow(SW_NORMAL); picNoAct8.ShowWindow(SW_HIDE);}
}


void PlayerWindow::SetInputSignalState(char input, char state){
	char s[8]={3,3,3,3,3,3,3,3};
	s[input] = state;
	SetInputSignalsStates(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]);
}


bool PlayerWindow::CreateNewProgram(CString name){
	return m_programPlayer.CreateNewProgram(name, true);
}


bool PlayerWindow::LoadProgramFromFile(CString name){
	return m_programPlayer.LoadProgramFromFile(name);
}


bool PlayerWindow::SaveProgram(){
	return m_programPlayer.SaveProgram();
}


bool PlayerWindow::AddNewSignal(unsigned char signalOrdinal, unsigned long startTime, unsigned long startDuration, bool startState, bool clearSignals){
	return m_programPlayer.AddNewSignal(signalOrdinal, startTime, startDuration, startState, clearSignals); 
}


bool PlayerWindow::AppendSignal(unsigned char signalOrdinal, unsigned long signalDuration){
	return m_programPlayer.AppendSignal(signalOrdinal, signalDuration);
}


bool PlayerWindow::InsertSingalAtIndex(unsigned char signalOrdinal, unsigned int signalIndex, unsigned long signalDuration){
	return m_programPlayer.InsertSingalAtIndex(signalOrdinal, signalIndex, signalDuration);
}


bool PlayerWindow::DeleteSignalAtIndex(unsigned char signalOrdinal, unsigned int signalIndex){
	return m_programPlayer.DeleteSignalAtIndex(signalOrdinal, signalIndex); 
}


unsigned int PlayerWindow::GetCountOfSignals(unsigned char signalOrdinal){
	return m_programPlayer.GetNumOfAppendedSignals(signalOrdinal);
}


void PlayerWindow::DrawProgram(bool highTimeResolution){
	m_programPlayer.SetTimeResolution(highTimeResolution);
	m_programPlayer.RedrawWindow();
	AdjustScrollBar();
}


bool PlayerWindow::IsProgramLoaded() {
	return m_programPlayer.IsProgramCreated();
}


unsigned long PlayerWindow::GetTotalDuration(){
	return m_programPlayer.GetTotalDuration();
}


unsigned long PlayerWindow::GetSignalDuration(unsigned char signalOrdinal, unsigned int signalIndex){
	return m_programPlayer.GetSignalDuration(signalOrdinal, signalIndex);
}


bool PlayerWindow::GetStartSignalData(unsigned char signalOrdinal, unsigned long *startTime, unsigned long *startDuration, bool *startState){
	*startDuration = m_programPlayer.GetSingalStartDuration(signalOrdinal); 
	if(*startDuration == 0)
		return false;

	*startTime = m_programPlayer.GetSingalStartTime(signalOrdinal);
	*startState = m_programPlayer.GetSignalStartState(signalOrdinal);
	return true;
}


bool PlayerWindow::AddNewInputMonitor(unsigned char input, Program::tag_InputActions action, CString *program, int loopProgram){
	return m_programPlayer.AddNewInputMonitor(input, action, program, loopProgram);
}


Program::tag_InputActions PlayerWindow::GetInputMonitorAction(unsigned char input, CString*program, int *loopProgram){
	return m_programPlayer.GetActionOnInput(input, program, loopProgram);
}


void PlayerWindow::SetTimeResolution(bool highTimeResolution){
	m_programPlayer.SetTimeResolution(highTimeResolution);
	m_programPlayer.RedrawWindow();
	AdjustScrollBar();
}


void PlayerWindow::Play(unsigned char loop, IIODevice *ioDevice){
	m_ioDevice = ioDevice;	
	m_programPlayer.Play(loop, this->GetSafeHwnd());
	cntScroll.EnableWindow(0);
}


LRESULT PlayerWindow::OnPlayerStop(WPARAM wParam, LPARAM lParam){
	m_programPlayer.RedrawWindow();
	txtTime.SetWindowTextW(_T(""));
	StopIoCkeck();

	if (AdjustScrollBar()) {
		cntScroll.SetScrollPos(cntScroll.GetScrollLimit());
	}
	return 1;
}


LRESULT PlayerWindow::OnTimeChange(WPARAM wParam, LPARAM lParam){
	CString time;
	time.Format(_T("%.3f sec / %.3f sec"), wParam/(float)1000, lParam/(float)1000);
	txtTime.SetWindowTextW(time);

	if (m_ioDevice) {
		DoIoCheck((unsigned long)wParam);
	}

	return 1;
}


LRESULT PlayerWindow::OnInput(WPARAM wParam, LPARAM lParam){
	int input = (int)wParam;
	CString program;
	int loop = 0;
	tag_IoActionThreadData *data;

	Program::tag_InputActions act = m_programPlayer.GetActionOnInput(input-1, &program, &loop);
	if(loop == 1)
		loop = 0;
	else 
		loop = 1;

	switch(act){
		case Program::InputAction_Beep:
			SetInputSignalState(input-1, 1);
			data = new tag_IoActionThreadData();
			data->parent = GetSafeHwnd();
			data->data = input-1;
			data->action = 1;
			AfxBeginThread(DoIoActionThread, data, THREAD_PRIORITY_BELOW_NORMAL);
			break;

		case Program::InputAction_Disabled:
			break;

		case Program::InputAction_Indicate:
			SetInputSignalState(input-1, 1);
			data = new tag_IoActionThreadData();
			data->parent = GetSafeHwnd();
			data->data = input-1;
			data->action = 0;
			AfxBeginThread(DoIoActionThread, data, THREAD_PRIORITY_BELOW_NORMAL);
			break;

		case Program::InputAction_PauseProgram:
			SetInputSignalState(input-1, 1);
			data = new tag_IoActionThreadData();
			data->parent = GetSafeHwnd();
			data->data = input-1;
			data->action = 0;
			AfxBeginThread(DoIoActionThread, data, THREAD_PRIORITY_BELOW_NORMAL);
			Pause();
			break;

		case Program::InputAction_SetOutputsToOne:
			Stop();
			m_ioDevice->SetOutput(0xff);
			break;

		case Program::InputAction_SetOutputsToZero:
			Stop();
			m_ioDevice->SetOutput(0);
			break;

		case Program::InputAction_StartProgram:
			Stop();

			if(!LoadProgramFromFile(program)){
				AfxMessageBox(_T("Unable to load program..."));
				return 0;
			}

			for(int i=0; i<8; i++)
				SetInputSignalState(i, GetInputMonitorAction(i) == 
					Program::InputAction_Disabled ? -1 : 0);

			if(GetTotalDuration()<12000)
				DrawProgram(true);
			else
				DrawProgram(false);

			Play((unsigned char)loop, m_ioDevice);
			break;

		case Program::InputAction_StopProgram:
			Stop();
			break;
	}

	return 1;
}


LRESULT PlayerWindow::OnResetInput(WPARAM wParam, LPARAM lParam){
	SetInputSignalState((char)wParam, 0);
	return 1;
}


UINT PlayerWindow::DoIoActionThread(LPVOID p){
	tag_IoActionThreadData *data = (tag_IoActionThreadData*)p;

	if(data->action == 0){ //reset input pics
		Sleep(1500);
		::SendMessage(data->parent, WM_USER+4, data->data, 0);
	}
	else if(data->action == 1){ //beep
		Beep(4500, 1500);
		::SendMessage(data->parent, WM_USER+4, data->data, 0);
	}

	delete data;
	return 1;
}


UINT PlayerWindow::IoCheckThread(LPVOID p){
	tag_IoThreadData *data = (tag_IoThreadData*)p;
	unsigned char signals=0;
	unsigned char output=0;
	unsigned char lastOutput=0;
	int outputCount=0;
	bool reset = true;

	while(InterlockedCompareExchange(&data->isThreadLooping, 1, 1)){
		InterlockedExchange(&data->isRunning, 1);

		if(data->time){		
			data->program->GetSignalStatesForTime(data->time, &signals);
			if (!data->ioDevice->SetOutput(signals)) {
				reset = true;
			}

			if(output = (data->ioDevice->GetInput(reset))){
				if(output != lastOutput){
					lastOutput = output;
					signals = 0;

					while(output){
						output = output >> 1;
						signals ++;
					}
				
					if (data->program->GetActionOnInput(signals - 1) != Program::InputAction_Disabled) {
						::SendMessage(data->parent, WM_USER + 3, signals, 0);
					}
					
					outputCount = 0;
				}
			}

			reset = false;
			data->time = 0;
		}

		InterlockedExchange(&data->isRunning, 0);

		if(++outputCount > 500){ //500 * 5ms = cca. 2,5 sec
			outputCount = 0;	
			lastOutput = 0;
		}
		Sleep(5);
	}
	
	InterlockedExchange(&data->isRunning, 0);
	InterlockedExchange(&data->isThreadLooping, 0);
	data->ioDevice = NULL;
	data->program = NULL;
	data->time = 0;
	return 1;
}


void PlayerWindow::DoIoCheck(unsigned long time){
	if(InterlockedCompareExchange(&m_ioThreadData.isThreadLooping, 1, 1)){
		if(InterlockedCompareExchange(&m_ioThreadData.isRunning, 1, 1))
			return;
		else
			m_ioThreadData.time = time;
	}
	else{
		m_ioThreadData.ioDevice = m_ioDevice;
		m_ioThreadData.isRunning = 0;
		m_ioThreadData.isThreadLooping = 1;
		m_ioThreadData.program = (Program*)&m_programPlayer;
		m_ioThreadData.time = 0;
		m_ioThreadData.parent = GetSafeHwnd();
		AfxBeginThread(IoCheckThread, &m_ioThreadData, THREAD_PRIORITY_NORMAL);
	}
}


void PlayerWindow::StopIoCkeck(){
	InterlockedExchange(&m_ioThreadData.isThreadLooping, 0);
}


void PlayerWindow::Stop(){
	m_programPlayer.Stop();
	StopIoCkeck();
	txtTime.SetWindowTextW(_T(""));
	if(AdjustScrollBar())
		cntScroll.SetScrollPos(0);
}


void PlayerWindow::Pause(){
	m_programPlayer.Pause();
}


void PlayerWindow::Resume(){
	m_programPlayer.Resume();
}


bool PlayerWindow::IsNowPlaying(){
	return m_programPlayer.IsPlaying();
}


bool PlayerWindow::AdjustScrollBar(){
	RECT area;
	long areaWidth = 0;

	areaWidth = m_programPlayer.GetPixelLenForDuration(m_programPlayer.GetTotalDuration());
	if(areaWidth >= 32000){ //programs that are longer than cca 12 sec can only be shwn on 1second time resolution
		m_programPlayer.SetTimeResolution(false);
		areaWidth = m_programPlayer.GetPixelLenForDuration(m_programPlayer.GetTotalDuration());
	}

	GetClientRect(&area);
	if(areaWidth==0 || area.right > areaWidth){
		cntScroll.EnableWindow(0);
		return false;
	}

	cntScroll.EnableWindow();
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMax = areaWidth - (area.right - 100);
	si.nMin = 0;
	si.nPage = 50;
	si.nPos = 0;
	si.nTrackPos = 1;

	cntScroll.SetScrollInfo(&si, 1);
	cntScroll.SetScrollPos(m_currentScrollPos); 
	return true;
}


void PlayerWindow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	int pos=0;
	
	if(nSBCode == SB_THUMBPOSITION){
		if(nPos<0xffff)
			cntScroll.SetScrollPos(nPos);
		else{
			pos = 0x0000FFFF & nPos;
			cntScroll.SetScrollPos(pos);	
		}
		m_programPlayer.SetScrollOffset(cntScroll.GetScrollPos());
	}
	else if(nSBCode == SB_PAGELEFT){
		cntScroll.SetScrollPos(cntScroll.GetScrollPos() - 50); 
		m_programPlayer.SetScrollOffset(cntScroll.GetScrollPos());
	}
	else if(nSBCode == SB_PAGERIGHT){
		cntScroll.SetScrollPos(cntScroll.GetScrollPos() + 50);
		m_programPlayer.SetScrollOffset(cntScroll.GetScrollPos());
	}
	else if(nSBCode == SB_LINELEFT){
		cntScroll.SetScrollPos(cntScroll.GetScrollPos() - 10);
		m_programPlayer.SetScrollOffset(cntScroll.GetScrollPos());
	}
	else if(nSBCode == SB_LINERIGHT){
		cntScroll.SetScrollPos(cntScroll.GetScrollPos() + 10);
		m_programPlayer.SetScrollOffset(cntScroll.GetScrollPos());
	}
	
	m_currentScrollPos = cntScroll.GetScrollPos();
}
