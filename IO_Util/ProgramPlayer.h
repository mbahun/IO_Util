#pragma once
#include "Program.h"

class ProgramPlayer : public CWnd, public Program{
	DECLARE_DYNAMIC(ProgramPlayer)
	
	private:
		int m_leftMargin;
		int m_leftOffset;
		int m_topMargin;
		int m_topOffset;
		int m_horizontalLineDistance;
		int m_verticalLineDistance;
		int m_timeSlotDuration;
		int m_currentSweepLinePosition;
		
		unsigned long m_backgroundColor;
		unsigned long m_axisTextColor;
		unsigned long m_horizontalLineColor;
		unsigned long m_verticalLineColor;
		unsigned long m_signalHorizontalLineColor;
		unsigned long m_signalVerticalLineColor;
		unsigned long m_sweepLineColor;
		unsigned char m_sweepLineThickness;

		unsigned char m_loopPlayer;
		unsigned long m_programDuration;
		unsigned long m_currentPlayTime;
		bool m_isPlaying;

		HWND m_playerWindow;	
	
	private:
		struct tag_ThreadData{
			HWND parentWindow;
			volatile long threadStop;
			volatile long threadRunning;
			volatile long doDelete;
		} *m_threadData;

	private:
		void DrawAxisText(CPaintDC *dc);
		void DrawGrid(CPaintDC *dc, RECT *area);
		void DrawSignal(CPaintDC *dc);
		void DrawSweepLine(CPaintDC *dc, RECT *area, unsigned long timePosition);

		static UINT LinePainterThread(LPVOID p);

	public:
		ProgramPlayer();
		virtual ~ProgramPlayer();
		unsigned long GetPixelLenForDuration (unsigned long duration);
		void SetScrollOffset(long offset);
		void SetTimeResolution(bool high);

		void Play(unsigned char loop, HWND playerWindow);
		void Stop();
		void Pause();
		void Resume();
		bool IsPlaying();

	private:
		LRESULT MoveSweepLine(WPARAM wParam, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

