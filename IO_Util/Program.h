#pragma once
#include "afxtempl.h"

class Program{
	public:
		enum tag_InputActions{
			InputAction_Disabled			= 0,
			InputAction_PauseProgram		= 1,
			InputAction_StopProgram			= 2,
			InputAction_StartProgram		= 4,
			InputAction_SetOutputsToZero	= 8,
			InputAction_SetOutputsToOne		= 16,
			InputAction_Beep				= 32,
			InputAction_Indicate			= 64
		};

	private:
		struct tag_Signal {
			unsigned long startTime = 0; //ms
			bool startState = false;
			unsigned long startDuration = 0; //ms
			bool initialised = false;
			CList<unsigned long, unsigned long> signals;
		} *m_signals = NULL;

		struct tag_Input{
			tag_InputActions action;
			wchar_t program[256];
			int programLoop;
		}m_inputMonitors[8];

		const unsigned char m_singalsInProgram;
		CString m_filename;

	private:
		bool AreSignalsCorrect(unsigned char signalOrdinal, bool createSignal = false);
		void DeleteSignals();
		bool DoOverwrite(CString file);
		unsigned char GetSignalStatesForTimeForSignal(unsigned char signalOrdinal, unsigned long time);

	public:
		Program(void);
		virtual ~Program(void);	

		bool CreateNewProgram(CString name, bool doOverwriteCheck=false);
		bool IsProgramCreated();
		bool LoadProgramFromFile(CString name);
		bool SaveProgram();

		bool AddNewSignal(unsigned char signalOrdinal, unsigned long startTime, unsigned long startDuration, bool startState, bool clearSignals = true);
		bool AppendSignal(unsigned char signalOrdinal, unsigned long signalDuration);
		bool InsertSingalAtIndex(unsigned char signalOrdinal, unsigned int signalIndex, unsigned long signalDuration);
		bool DeleteSignalAtIndex(unsigned char signalOrdinal, unsigned int signalIndex);

		bool AddNewInputMonitor(unsigned char input, tag_InputActions action, CString *program=NULL, int loopProgram=0);
		Program::tag_InputActions GetActionOnInput(unsigned char input, CString *program=NULL, int *loopProgram=NULL);

		bool IsSignalActive(unsigned char singalOrdinal);
		unsigned char GetNumberOfSignals();
		unsigned int GetNumOfAppendedSignals(unsigned char singalOrdinal);
		
		unsigned long GetSingalStartTime(unsigned char signalOrdinal);
		unsigned long GetSingalStartDuration(unsigned char signalOrdinal);
		bool GetSignalStartState(unsigned char signalOrdinal);
		void GetSignalStatesForTime(unsigned long time, unsigned char *signals);
		unsigned long GetSignalDuration(unsigned char signalOrdinal, unsigned int appendedSignalIndex);
		unsigned long GetTotalDuration();
};

