#pragma once
#include "afxwin.h"
#include "ProgramPlayer.h"
#include "IIODevice.h"


class PlayerWindow : public CDialog{
	DECLARE_DYNAMIC(PlayerWindow)
	private:
		struct tag_IoThreadData{
			IIODevice* ioDevice;
			Program	*program;
			HWND parent;
			volatile unsigned long time;
			volatile long isRunning;
			volatile long isThreadLooping;
		} m_ioThreadData;

		struct tag_IoActionThreadData{
			HWND parent;
			int action;
			int data;
		};

		static UINT IoCheckThread(LPVOID p);
		static UINT DoIoActionThread(LPVOID p);

		void DoIoCheck(unsigned long time);
		void StopIoCkeck();
		void ArrangeInputPics();

	public:
		PlayerWindow();
		virtual ~PlayerWindow();
		enum {IDD = IDD_PLAYER_WINDOW};

		void Create(CWnd *parent, int parentWidth, int rightMargin = 0, int leftMargin = 0, int topMargin = 0);
		void SetNewWidth(int parentWidth);

		bool CreateNewProgram(CString name);
		bool LoadProgramFromFile(CString name);
		bool SaveProgram();
		void DrawProgram(bool highTimeResolution=false);
		bool IsProgramLoaded();

		bool AddNewSignal(unsigned char signalOrdinal, unsigned long startTime, unsigned long startDuration, bool startState, bool clearSignals = true);
		bool AppendSignal(unsigned char signalOrdinal, unsigned long signalDuration);
		bool InsertSingalAtIndex(unsigned char signalOrdinal, unsigned int signalIndex, unsigned long signalDuration);
		bool DeleteSignalAtIndex(unsigned char signalOrdinal, unsigned int signalIndex);
		unsigned int GetCountOfSignals(unsigned char signalOrdinal);
		unsigned long GetTotalDuration();
		void SetTimeResolution(bool highTimeResolution=false);
		unsigned long GetSignalDuration(unsigned char signalOrdinal, unsigned int signalIndex);
		bool GetStartSignalData(unsigned char signalOrdinal, unsigned long *startTime, unsigned long *startDuration, bool *startState);
		bool AddNewInputMonitor(unsigned char input, Program::tag_InputActions action, CString*program=NULL, int loopProgram=0);
		Program::tag_InputActions GetInputMonitorAction(unsigned char input, CString *program=NULL, int *loopProgram=NULL);
		void SetInputSignalsStates(char s1, char s2, char s3, char s4, char s5, char s6, char s7, char s8);
		void SetInputSignalState(char input, char state);

		void Play(unsigned char loop, IIODevice *ioDevice=NULL);
		void Stop();
		void Pause();
		void Resume();
		bool IsNowPlaying();

	private:
		bool AdjustScrollBar();
		IIODevice *m_ioDevice;

	private:
		ProgramPlayer m_programPlayer;
		CScrollBar cntScroll;
		CEdit txtTime;

		CStatic picDis1;
		CStatic picDis2;
		CStatic picDis3;
		CStatic picDis4;
		CStatic picDis5;
		CStatic picDis6;
		CStatic picDis7;
		CStatic picDis8;
		CStatic picAct1;
		CStatic picAct2;
		CStatic picAct3;
		CStatic picAct4;
		CStatic picAct5;
		CStatic picAct6;
		CStatic picAct7;
		CStatic picAct8;
		CStatic picNoAct1; 
		CStatic picNoAct2;
		CStatic picNoAct3;
		CStatic picNoAct4;
		CStatic picNoAct5;
		CStatic picNoAct6;
		CStatic picNoAct7;
		CStatic picNoAct8;

		int m_height;
		int m_width;
		int m_topMargin; 
		int m_leftMargin;
		int m_rightMargin;
		int m_inputSpaceHeight;
		unsigned int m_currentScrollPos;
		
	private:
		virtual BOOL OnInitDialog();
		virtual void DoDataExchange(CDataExchange* pDX); 

		LRESULT OnPlayerStop(WPARAM wParam, LPARAM lParam);
		LRESULT OnTimeChange(WPARAM wParam, LPARAM lParam);
		LRESULT OnInput(WPARAM wParam, LPARAM lParam);
		LRESULT OnResetInput(WPARAM wParam, LPARAM lParam);

		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);	
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);	

	DECLARE_MESSAGE_MAP()
};
