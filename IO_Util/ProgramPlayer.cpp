#include "stdafx.h"
#include "ProgramPlayer.h"
#include "Mmsystem.h"

#pragma comment (lib, "Winmm.lib")

IMPLEMENT_DYNAMIC(ProgramPlayer, CWnd)

ProgramPlayer::ProgramPlayer(){
	m_leftMargin = 25;
	m_leftOffset = 0;
	m_topMargin = 40;
	m_topOffset = 20;
	m_horizontalLineDistance = 40;
	m_verticalLineDistance = 50;
	m_currentSweepLinePosition = 0;

	m_isPlaying = false;
	m_loopPlayer = 0;
	m_programDuration = 0;
	m_currentPlayTime = 0;
	m_playerWindow = NULL;
	m_timeSlotDuration = 20; //20 ms or 1000=1s

	m_sweepLineThickness = 6;
	m_sweepLineColor = RGB(20, 20, 255);
	m_backgroundColor = RGB(18, 8, 32);
	m_axisTextColor = RGB(0, 64, 64);
	m_horizontalLineColor = RGB(0, 0, 200);
	m_verticalLineColor = RGB(0, 0, 200);
	m_signalHorizontalLineColor = RGB(100, 32, 128);
	m_signalVerticalLineColor = RGB(100, 0, 128);

	m_threadData = new tag_ThreadData();
}


ProgramPlayer::~ProgramPlayer(){
	if(InterlockedCompareExchange(&m_threadData->threadRunning, 1, 1))
		InterlockedExchange(&m_threadData->doDelete, 1);
	else
		delete m_threadData;
}


BEGIN_MESSAGE_MAP(ProgramPlayer, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER+1, MoveSweepLine)
END_MESSAGE_MAP()


void ProgramPlayer::OnSize(UINT nType, int cx, int cy){
	CWnd::OnSize(nType, cx, cy);
}


void ProgramPlayer::SetTimeResolution(bool high){
	if(high)
		m_timeSlotDuration = 20;
	else
		m_timeSlotDuration = 1000;

	SetScrollOffset(0); 
	RedrawWindow();
}


unsigned long ProgramPlayer::GetPixelLenForDuration(unsigned long miliseconds){
	if(m_timeSlotDuration == 20) //time unit is 20ms, 20ms = m_verticalLineDistance pixels
		return (miliseconds / 20) * m_verticalLineDistance;
	else //time unit is 1s, 1s = m_verticalLineDistance pixels
		return miliseconds / 20;
}


void ProgramPlayer::OnPaint(){
	CPaintDC dc(this);
	RECT area, tempArea;

	GetClientRect(&area);

	if(m_currentSweepLinePosition == 0)
		dc.FillSolidRect(&area, m_backgroundColor);
	else{
		tempArea.left = 0;
		tempArea.top = 0;
		tempArea.right = m_leftMargin + m_currentSweepLinePosition;
		tempArea.bottom = area.bottom;
		dc.FillSolidRect(&tempArea, m_backgroundColor);
		tempArea.left = tempArea.right + m_sweepLineThickness; //line thickness
		tempArea.right = area.right;
		dc.FillSolidRect(&tempArea, m_backgroundColor);
	}

	DrawGrid(&dc, &area);

	area.right = m_leftMargin;
	area.top = m_topOffset;
	dc.FillSolidRect(&area, m_backgroundColor);
	dc.SetBkColor(m_backgroundColor);

	if (IsProgramCreated()) {
		DrawSignal(&dc);
		DrawAxisText(&dc);
	}

	if (m_isPlaying) {
		DrawSweepLine(&dc, &area, m_currentSweepLinePosition);
	}
}


void ProgramPlayer::DrawAxisText(CPaintDC *dc){
	CFont fnt;
	CString text;
	RECT rect;

	rect.left	= m_leftMargin - 18;
	rect.top	= m_topMargin - 5;
	rect.right	= 500;
	rect.bottom	= 500;

	fnt.CreateFontW(13, 8, 0, 0, 800, 0, 0, 0, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("VERDANA\0"));

	dc->SetTextColor(m_axisTextColor);
	dc->SelectObject(fnt);

	for(int i=1; i<=8; i++){
		text.Format(_T("%i\0"), i);
		dc->DrawTextW(text, -1, &rect, DT_LEFT);
		rect.top += m_horizontalLineDistance;
	}
	fnt.DeleteObject();
}


void ProgramPlayer::DrawGrid(CPaintDC *dc, RECT *area){
	CPen pen;
	CString txt;
	RECT rect;

	pen.CreatePen(PS_DOT, 1, m_horizontalLineColor);
	dc->SelectObject(pen);
	
	//draw horizontal lines
	for(int i = 0; i<8; i++){
		dc->MoveTo(m_leftMargin, m_topMargin + (i * m_horizontalLineDistance));
		dc->LineTo(area->right, m_topMargin + (i * m_horizontalLineDistance));
	}
	pen.DeleteObject();

	//draw vertical lines
	pen.CreatePen(PS_DOT, 1, m_verticalLineColor);
	dc->SelectObject(pen);
	for(int i = 0; i<area->right + m_leftOffset; i+=m_verticalLineDistance){
		if ((m_leftMargin + i - m_leftOffset) < m_leftMargin) {
			continue;
		}
		dc->MoveTo(m_leftMargin + (i - m_leftOffset), m_topMargin - 25);
		dc->LineTo(m_leftMargin + (i - m_leftOffset), area->bottom	- 5);
	}
	pen.DeleteObject();
	
	//draw horizontal axis (time) text
	CFont fnt;
	fnt.CreateFontW(12, 6, 0, 0, 100, 0, 0, 0, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("VERDANA\0"));

	dc->SetTextColor(m_axisTextColor);
	dc->SelectObject(fnt);

	rect.top		= 5;
	rect.bottom		= 500;
	rect.right = area->right;

	for(int i = 1; i<area->right + m_leftOffset; i+=m_verticalLineDistance){
		m_timeSlotDuration == 20 ? 
			txt.Format(_T("%i ms"), i/m_verticalLineDistance*20) : 
			txt.Format(_T("%i s"), i/m_verticalLineDistance);

		if ((m_leftMargin + i - m_leftOffset) < m_leftMargin) {
			continue;
		}

		rect.left = m_leftMargin -3+i - m_leftOffset;
		dc->DrawTextW(txt, -1, &rect, DT_LEFT);
	}
	fnt.DeleteObject();
}


void ProgramPlayer::DrawSignal(CPaintDC *dc){
	int upPos = 0;
	int downPos = 0;
	long leftPos = m_leftMargin;
	long duration = 0;
	bool nextState = false;
	long currentLeftOffset = 0;

	CPen horizontalPen, verticalPen;
	horizontalPen.CreatePen(PS_SOLID, 2, m_signalHorizontalLineColor);
	verticalPen.CreatePen(PS_SOLID, 1, m_signalVerticalLineColor);

	dc->SelectObject(horizontalPen);
	for(unsigned char i=0; i<GetNumberOfSignals(); i++){
		upPos	= (m_topMargin + (i * m_horizontalLineDistance)) - 16;
		downPos = upPos + 32;
		leftPos = m_leftMargin;

		if (!IsSignalActive(i)) {
			continue;
		}

		for(unsigned int j=0; j<=GetNumOfAppendedSignals(i); j++){
			if(j==0){
				leftPos += GetPixelLenForDuration(GetSingalStartTime(i));
				leftPos -= m_leftOffset;

				GetSignalStartState(i) ? dc->MoveTo(leftPos, upPos) : dc->MoveTo(leftPos, downPos);
				duration = GetPixelLenForDuration(GetSingalStartDuration(i)) + leftPos;
				GetSignalStartState(i) ? dc->LineTo(duration, upPos) : dc->LineTo(duration, downPos);

				leftPos = duration;
				nextState = !GetSignalStartState(i);
			}
			else{
				dc->SelectObject(verticalPen);
				nextState ? dc->MoveTo(leftPos, downPos) : dc->MoveTo(leftPos, upPos);
				nextState ? dc->LineTo(leftPos, upPos) : dc->LineTo(leftPos, downPos);

				dc->SelectObject(horizontalPen);
				nextState ? dc->MoveTo(leftPos, upPos) : dc->MoveTo(leftPos, downPos);
				duration = GetPixelLenForDuration(GetSignalDuration(i,j-1)) + leftPos;
				nextState ? dc->LineTo(duration, upPos) : dc->LineTo(duration, downPos);
				leftPos = duration;
				nextState = !nextState;
			}
		}
	}
	horizontalPen.DeleteObject();
	verticalPen.DeleteObject();
}


void ProgramPlayer::DrawSweepLine(CPaintDC *dc, RECT *area, unsigned long timePosition){
	CPen pen;
	int r = 0, g = 0, b = 0;
	unsigned char backR = 0, backG = 0, backB = 0;
	unsigned char lineR = 0, lineG = 0, lineB = 0;

	backR = (unsigned char)m_backgroundColor;
	backG = (unsigned char)(m_backgroundColor >> 8);
	backB = (unsigned char)(m_backgroundColor >> 16);

	lineR = (unsigned char)m_sweepLineColor;
	lineG = (unsigned char)(m_sweepLineColor >> 8);
	lineB = (unsigned char)(m_sweepLineColor >> 16);

	r = (backR - lineR) / m_sweepLineThickness;
	g = (backG - lineG) / m_sweepLineThickness;
	b = (backB - lineB) / m_sweepLineThickness;
	
	for(int i=0; i<m_sweepLineThickness; i++){		
		pen.CreatePen(PS_SOLID, 1, RGB(
			lineR + (r * (m_sweepLineThickness-i)), 
			lineG + (g * (m_sweepLineThickness-i)), 
			lineB + (b * (m_sweepLineThickness-i)))
		);

		dc->SelectObject(pen);
		dc->MoveTo(m_leftMargin + timePosition + i, area->top-25);
		dc->LineTo(m_leftMargin + timePosition + i, area->bottom);
		pen.DeleteObject();
	}
}


void ProgramPlayer::SetScrollOffset(long offset){
	m_leftOffset = offset;
	RedrawWindow();
}


void ProgramPlayer::Play(unsigned char loop, HWND playerWindow){
	if (m_isPlaying || !IsProgramCreated()) {
		return;
	}

	m_programDuration = GetTotalDuration();
	m_loopPlayer = 0;
	m_currentPlayTime = 0;

	SetTimeResolution(false);
	SetScrollOffset(0);
	m_loopPlayer = loop;
	m_currentPlayTime = 0;
	m_currentSweepLinePosition = 0;
	m_isPlaying = true;
	m_playerWindow = playerWindow;

	TIMECAPS tc;
	CString txt; 
	timeGetDevCaps(&tc, sizeof(TIMECAPS));
	if(tc.wPeriodMin > 1){
		txt.Format(_T("Cannot set minimum time resolution to 1ms. Min. resolution for ")
			_T("this PC is: %i. Timings will be wrong..."), tc.wPeriodMin);

		AfxMessageBox(txt);
	}

	m_threadData->parentWindow = GetSafeHwnd();
	m_threadData->threadStop = 0;
	AfxBeginThread(LinePainterThread, m_threadData, THREAD_PRIORITY_BELOW_NORMAL);
	return;
}


void ProgramPlayer::Stop(){
	if(!m_isPlaying)
		return;

	InterlockedExchange(&m_threadData->threadStop, 1);
	m_isPlaying = false;
	SetScrollOffset(0);
	m_currentSweepLinePosition = 0;
	RedrawWindow();
}


void ProgramPlayer::Pause(){
	if(!m_isPlaying)
		return;
	
	InterlockedExchange(&m_threadData->threadStop, 1);
}


void ProgramPlayer::Resume(){
	if(InterlockedCompareExchange(&m_threadData->threadRunning, 1, 1))
		return;

	if(!m_isPlaying)
		return;

	m_threadData->parentWindow = GetSafeHwnd();
	m_threadData->threadStop = 0;
	AfxBeginThread(LinePainterThread, m_threadData, THREAD_PRIORITY_BELOW_NORMAL);
}


bool ProgramPlayer::IsPlaying(){
	return m_isPlaying;
}


LRESULT ProgramPlayer::MoveSweepLine(WPARAM wParam, LPARAM lParam){
	m_currentSweepLinePosition += 1; //steps to move! (1 pixel every 20ms)
	m_currentPlayTime += 20;

	RECT area;
	GetClientRect(&area);

	if(m_currentPlayTime > m_programDuration){
		if(m_loopPlayer == 1){
			Stop();
			::SendMessage(m_playerWindow, WM_USER+1, 0, 0);
			return 1;
		}
		else{
			if(m_loopPlayer)
				m_loopPlayer--;

			SetScrollOffset(0);
			m_currentSweepLinePosition = 0;
			m_currentPlayTime = 0;
			InvalidateRect(&area, 0);
		}
	}
	
	if(m_currentSweepLinePosition >= area.right - m_leftMargin){
		SetScrollOffset(m_currentSweepLinePosition + m_leftOffset);
		m_currentSweepLinePosition = 0;
	}

	area.left = m_currentSweepLinePosition + m_leftMargin - 1;
	area.right = area.left + 8;
	InvalidateRect(&area, 0);
	::PostMessage(m_playerWindow, WM_USER+2, m_currentPlayTime, m_programDuration);
	return 1;
}


UINT ProgramPlayer::LinePainterThread(LPVOID p){
	tag_ThreadData *td = (tag_ThreadData*)p;
	InterlockedExchange(&td->threadRunning, 1);

	timeBeginPeriod(1);
	while(!InterlockedCompareExchange(&td->threadStop, 1, 1)){
		::SendMessage(td->parentWindow, WM_USER+1, 0, 0);
		Sleep(20);
	}
	timeEndPeriod(1);

	if(InterlockedCompareExchange(&td->doDelete, 1, 1))
		delete td;

	InterlockedExchange(&td->threadRunning, 0);
	return 1;
}

