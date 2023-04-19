#include "StdAfx.h"
#include "Program.h"


Program::Program(void) : m_singalsInProgram(8){
	m_signals = NULL;
	RtlZeroMemory(m_inputMonitors, sizeof(tag_Input) * 8);
}


Program::~Program(void){
	DeleteSignals();
}


void Program::DeleteSignals(){
	if(m_signals){
		for(int i=0; i<m_singalsInProgram; i++)
			m_signals[i].signals.RemoveAll();

		delete[] m_signals;
		m_signals = NULL;
	}
}


bool Program::AreSignalsCorrect(unsigned char signalOrdinal, bool createSignal){
	if(!m_signals || signalOrdinal >= m_singalsInProgram)
		return false;

	if(!createSignal){
		if(!m_signals[signalOrdinal].initialised)
			return false;
	}
	return true;
}


unsigned long Program::GetTotalDuration(){
	unsigned long len = 0, maxLen = 0;
	if(m_signals == NULL)
		return 0;

	for(unsigned char i=0; i<m_singalsInProgram; i++){
		if(!m_signals[i].initialised)
			continue;

		len = m_signals[i].startTime + m_signals[i].startDuration;

		for(unsigned int j=0; j<GetNumOfAppendedSignals(i); j++){
			len += GetSignalDuration(i, j);
		}
		if(len>maxLen)
			maxLen = len;
	}
	return maxLen;
}


unsigned char Program::GetSignalStatesForTimeForSignal(unsigned char signalOrdinal, unsigned long time){
	unsigned long totalDuration = 0;
	bool state = false;
	POSITION pos;

	if (m_signals == NULL) {
		return 0;
	}

	if(!m_signals[signalOrdinal].initialised || m_signals[signalOrdinal].startTime > time){
		return 0; 
	}

	state = m_signals[signalOrdinal].startState;
	totalDuration = m_signals[signalOrdinal].startTime + m_signals[signalOrdinal].startDuration;
	if(totalDuration >= time){
		return state ? 1 : 0;
	}

	if(!(pos = m_signals[signalOrdinal].signals.GetHeadPosition())){
		return 0;
	}
	
	while(pos){
		totalDuration += m_signals[signalOrdinal].signals.GetNext(pos);
		state = !state;
		if(totalDuration >= time){
			return state ? 1 : 0; 
		}
	}
	return 0;
}


void Program::GetSignalStatesForTime(unsigned long time, unsigned char *signals){
	*signals = 0;
	*signals =	GetSignalStatesForTimeForSignal(0, time) << 0 | 
				GetSignalStatesForTimeForSignal(1, time) << 1 | 
				GetSignalStatesForTimeForSignal(2, time) << 2 | 
				GetSignalStatesForTimeForSignal(3, time) << 3 | 
				GetSignalStatesForTimeForSignal(4, time) << 4 | 
				GetSignalStatesForTimeForSignal(5, time) << 5 | 
				GetSignalStatesForTimeForSignal(6, time) << 6 |
				GetSignalStatesForTimeForSignal(7, time) << 7;

}


bool Program::CreateNewProgram(CString name, bool doOverwriteCheck){
	CString filename;
	if(doOverwriteCheck){
		filename.Format(_T("Programs\\%s.iop"), name.GetBuffer());
		if(!DoOverwrite(filename.GetBuffer())){
			return false;
		}
	}

	DeleteSignals();
	unsigned int size = wcslen(name);
	if(size>255)
		return false;

	m_filename = name;
	m_signals = new tag_Signal[m_singalsInProgram];

	for(unsigned char i=0; i<m_singalsInProgram; i++){
		m_signals[i].initialised = false;
		m_signals[i].startDuration = 0;
		m_signals[i].startState = false;
		m_signals[i].startTime = 0;
	}

	for(unsigned char i=0; i<8; i++){
		m_inputMonitors[i].action = InputAction_Disabled;
		m_inputMonitors[i].programLoop = 0;
		RtlZeroMemory(m_inputMonitors[i].program, 512);
	}
		
	return true;
}


bool Program::LoadProgramFromFile(CString name){
	CString filename;
	unsigned long nbr = 0;
	bool boolValue;
	unsigned long longValue;
	unsigned int intValue;

	filename.Format(_T("Programs\\%s.iop"), name.GetBuffer());
	
	HANDLE file = CreateFileW(filename, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 0);

	if(file == INVALID_HANDLE_VALUE){
		return false;
	}

	CreateNewProgram(name);
	
	for(unsigned char i=0; i<m_singalsInProgram; i++){
		boolValue = false;
		if(!ReadFile(file, &boolValue, sizeof(bool), &nbr, 0) || nbr != sizeof(bool)){
			CloseHandle(file);
			return false;
		}
		m_signals[i].initialised = boolValue;
		
		longValue = 0;
		if(!ReadFile(file, &longValue, sizeof(long), &nbr, 0) || nbr != sizeof(long)){
			CloseHandle(file);
			return false;
		}
		m_signals[i].startDuration = longValue;

		boolValue = false;
		if(!ReadFile(file, &boolValue, sizeof(bool), &nbr, 0) || nbr != sizeof(bool)){
			CloseHandle(file);
			return false;
		}
		m_signals[i].startState = boolValue;

		longValue = 0;
		if(!ReadFile(file, &longValue, sizeof(long), &nbr, 0) || nbr != sizeof(long)){
			CloseHandle(file);
			return false;
		}
		m_signals[i].startTime = longValue;

		intValue = 0;
		if(!ReadFile(file, &intValue, sizeof(int), &nbr, 0) || nbr != sizeof(int)){
			CloseHandle(file);
			return false;
		}

		for(unsigned int j=0; j<intValue; j++){
			longValue = 0; //GetSignalDuration(i, j);
			if(!ReadFile(file, &longValue, sizeof(long), &nbr, 0) || nbr != sizeof(long)){
				CloseHandle(file);
				return false;
			}
			m_signals[i].signals.AddTail(longValue);
		}
	}

	//input monitor
	for(int i=0; i<8; i++){
		intValue = 0;
		if(!ReadFile(file, &intValue, sizeof(int), &nbr, 0) || nbr != sizeof(int)){
			continue;
		}
		m_inputMonitors[i].action = (tag_InputActions)intValue;

		m_inputMonitors[i].programLoop = 0;
		if(!ReadFile(file, &m_inputMonitors[i].programLoop, sizeof(int), &nbr, 0) || nbr != sizeof(int)){
			CloseHandle(file);
			return false;
		}

		RtlZeroMemory(&m_inputMonitors[i].program[0], 512);
		if(!ReadFile(file, &m_inputMonitors[i].program, 512, &nbr, 0) || nbr != 512){
			CloseHandle(file);
			return false;
		}
	}
	
	CloseHandle(file);
	return true;
}


bool Program::DoOverwrite(CString file){
	HANDLE hfile=NULL;
	WIN32_FIND_DATAW wfd;
	hfile = FindFirstFile(file.GetBuffer(), &wfd);

	if(hfile == INVALID_HANDLE_VALUE)
		return true;
	
	FindClose(hfile);
	CString txt = _T("Program with this name already exist. Do you want to overwrite it?");
	if (MessageBox(0, txt, _T("Overwrite"), MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES) {
		return true;
	}

	return false;
}


bool Program::SaveProgram(){
	CreateDirectory(_T("Programs"), 0);
	CString filename;
	unsigned long nbr = 0;
	bool boolValue;
	unsigned long longValue;
	unsigned int intValue;

	filename.Format(_T("Programs\\%s.iop"), m_filename.GetBuffer());
	
	HANDLE file = CreateFileW(filename, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 0);

	if(file == INVALID_HANDLE_VALUE){
		DWORD err = GetLastError();
		return false;
	}

	for(unsigned char i=0; i<m_singalsInProgram; i++){
		boolValue = IsSignalActive(i);
		if(!WriteFile(file, &boolValue, sizeof(bool), &nbr, 0) || nbr != sizeof(bool)){
			CloseHandle(file);
			DeleteFile(filename);
			return false;
		}

		longValue = GetSingalStartDuration(i);
		if(!WriteFile(file, &longValue, sizeof(long), &nbr, 0) || nbr != sizeof(long)){
			CloseHandle(file);
			DeleteFile(filename);
			return false;
		}

		boolValue = GetSignalStartState(i);
		if(!WriteFile(file, &boolValue, sizeof(bool), &nbr, 0) || nbr != sizeof(bool)){
			CloseHandle(file);
			DeleteFile(filename);
			return false;
		}

		longValue = GetSingalStartTime(i);
		if(!WriteFile(file, &longValue, sizeof(long), &nbr, 0) || nbr != sizeof(long)){
			CloseHandle(file);
			return false;
		}

		intValue = GetNumOfAppendedSignals(i);
		if(!WriteFile(file, &intValue, sizeof(int), &nbr, 0) || nbr != sizeof(int)){
			CloseHandle(file);
			return false;
		}

		for(unsigned int j=0; j<intValue; j++){
			longValue = GetSignalDuration(i, j);
			if(!WriteFile(file, &longValue, sizeof(long), &nbr, 0) || nbr != sizeof(long)){
				CloseHandle(file);
				DeleteFile(filename);
				return false;
			}
		}
	}

	//input monitors
	for(int i=0; i<8; i++){
		intValue = (int)m_inputMonitors[i].action;
		if(!WriteFile(file, &intValue, sizeof(int), &nbr, 0) || nbr != sizeof(int)){
			CloseHandle(file);
			DeleteFile(filename);
			return false;
		}

		intValue = m_inputMonitors[i].programLoop;
		if(!WriteFile(file, &intValue, sizeof(int), &nbr, 0) || nbr != sizeof(int)){
			CloseHandle(file);
			DeleteFile(filename);
			return false;
		}

		if(!WriteFile(file, &m_inputMonitors[i].program, 512, &nbr, 0) || nbr != 512){
			CloseHandle(file);
			DeleteFile(filename);
			return false;
		}
	}

	CloseHandle(file);
	return true;
}


bool Program::AddNewSignal(unsigned char signalOrdinal, unsigned long startTime, unsigned long startDuration, bool startState, bool clearSignals){
	if(!AreSignalsCorrect(signalOrdinal, true))
		return false;

	m_signals[signalOrdinal].initialised = true;
	m_signals[signalOrdinal].startDuration = startDuration;
	m_signals[signalOrdinal].startState = startState;
	m_signals[signalOrdinal].startTime = startTime;
	if(clearSignals)
		m_signals[signalOrdinal].signals.RemoveAll();

	return true;
}


bool Program::AppendSignal(unsigned char signalOrdinal, unsigned long signalDuration){
	if(!AreSignalsCorrect(signalOrdinal))
		return false;
	
	m_signals[signalOrdinal].signals.AddTail(signalDuration);
	return true;
}


bool Program::InsertSingalAtIndex(unsigned char signalOrdinal, unsigned int signalIndex, unsigned long signalDuration){
	if(!AreSignalsCorrect(signalOrdinal))
		return false;

	if(signalIndex >= (unsigned int)m_signals->signals.GetCount()-1)
		return false;
	
	m_signals[signalOrdinal].signals.InsertAfter(m_signals[signalOrdinal].signals.FindIndex(signalIndex), 
		signalDuration);
	return true;
}


bool Program::DeleteSignalAtIndex(unsigned char signalOrdinal, unsigned int signalIndex){
	if(!AreSignalsCorrect(signalOrdinal))
		return false;

	if(signalIndex > (unsigned int)m_signals->signals.GetCount()-1)
		return false;

	m_signals[signalOrdinal].signals.RemoveAt(m_signals[signalOrdinal].signals.FindIndex(signalIndex)); 
	return true;
}


bool Program::AddNewInputMonitor(unsigned char input, tag_InputActions action, CString *program, int loopProgram){
	m_inputMonitors[input].action = action;
	if(program){
		if ((*program).GetLength() < 256) {
			RtlZeroMemory(m_inputMonitors[input].program, 512);
			RtlCopyMemory(m_inputMonitors[input].program, program->GetBuffer(), (*program).GetLength() * 2);
		}
	}
	return true;
}


Program::tag_InputActions Program::GetActionOnInput(unsigned char input, CString *program, int *loopProgram){
	tag_InputActions retAct = m_inputMonitors[input].action;

	if(retAct == InputAction_StartProgram){
		if(program){
			if ((*program).GetLength() < 256) {
				RtlZeroMemory(program, 512);
				RtlCopyMemory(program, m_inputMonitors[input].program, (*program).GetLength() * 2);
			}

			if(loopProgram)
				*loopProgram = m_inputMonitors[input].programLoop;
		}
		else{
			retAct = InputAction_Indicate;
		}
	}

	return retAct;
}



bool Program::IsProgramCreated(){
	if(m_signals)
		return true;
	else
		return false;
}


unsigned char Program::GetNumberOfSignals(){
	if(m_signals)
		return m_singalsInProgram;
	else
		return 0;
}


bool Program::IsSignalActive(unsigned char signalOrdinal){
	if(AreSignalsCorrect(signalOrdinal) && IsProgramCreated())
		return m_signals[signalOrdinal].initialised;
	else
		return false;
}


unsigned int Program::GetNumOfAppendedSignals(unsigned char signalOrdinal){
	if(AreSignalsCorrect(signalOrdinal))
		return (unsigned int)m_signals[signalOrdinal].signals.GetCount();
	else
		return 0;
}


unsigned long Program::GetSingalStartTime(unsigned char signalOrdinal){
	if(AreSignalsCorrect(signalOrdinal))
		return m_signals[signalOrdinal].startTime;
	else
		return 0;
}


unsigned long Program::GetSingalStartDuration(unsigned char signalOrdinal){
	if(AreSignalsCorrect(signalOrdinal))
		return m_signals[signalOrdinal].startDuration;
	else
		return 0;
}


bool Program::GetSignalStartState(unsigned char signalOrdinal){
	if(AreSignalsCorrect(signalOrdinal))
		return m_signals[signalOrdinal].startState;
	else
		return false;
}


unsigned long Program::GetSignalDuration(unsigned char signalOrdinal, unsigned int appendedSignalIndex){
	if(AreSignalsCorrect(signalOrdinal)){
		if(appendedSignalIndex < (unsigned int)m_signals[signalOrdinal].signals.GetCount()) 
			return m_signals[signalOrdinal].signals.GetAt(m_signals[signalOrdinal].signals.FindIndex(appendedSignalIndex));
		else
			return 0;
	}
	else{
		return 0;
	}
}


