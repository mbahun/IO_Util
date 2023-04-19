#include "StdAfx.h"
#include "SerialPort.h"


SerialPort::SerialPort(void){
	m_port = NULL;
}


SerialPort::~SerialPort(void){
	Close();
}	


unsigned char SerialPort::SearchSerialPorts(unsigned char fromPort){
	unsigned char i = fromPort;
	CString strcom;
	HANDLE port;

	for(i; i<255; i++){
		strcom.Format(_T("\\\\.\\com%i"), i);
		port = NULL;
		port = CreateFileW(strcom, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (port != INVALID_HANDLE_VALUE && port != NULL){
			CloseHandle(port);
			return i;
		}
	}
	return 0;
}


bool SerialPort::Open(CString commPort) {
	unsigned char port = (unsigned char)_wtoi(commPort);
	return Open(port, 57600);
}


bool SerialPort::Open(unsigned char commPort, unsigned long baud){
	CString strcom;
	DCB dcb;
	COMMTIMEOUTS tmc;

	strcom.Format(_T("\\\\.\\com%i"), commPort);
	if(m_port){
		Close();
	}
	
	m_port = CreateFile(strcom, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 
		FILE_FLAG_NO_BUFFERING, NULL);

	if(m_port==INVALID_HANDLE_VALUE || m_port==NULL){
		m_port = NULL;
		return false;
	}

	dcb.fBinary = TRUE; 
	dcb.fParity = FALSE; 
	dcb.fOutxCtsFlow = FALSE; 
	dcb.fOutxDsrFlow = FALSE; 
	dcb.fDtrControl = DTR_CONTROL_ENABLE; 
	dcb.fDsrSensitivity = FALSE; 
	dcb.fTXContinueOnXoff = TRUE; 
	dcb.fOutX = FALSE; 
	dcb.fInX = FALSE; 
	dcb.fErrorChar = FALSE; 
	dcb.fNull = FALSE; 
	dcb.fRtsControl = RTS_CONTROL_DISABLE; 
	dcb.fAbortOnError = FALSE; 
	dcb.XonLim = 1; 
	dcb.XoffLim = 1024; 
	dcb.XonChar = 0; 
	dcb.XoffChar = 0; 
	dcb.ErrorChar = 0; 
	dcb.EofChar = 0; 
	dcb.EvtChar = 0; 
	dcb.BaudRate = baud; 
	dcb.ByteSize = 8; 
	dcb.Parity = NOPARITY; 
	dcb.StopBits = ONESTOPBIT; 

	tmc.ReadIntervalTimeout = 30;
	tmc.ReadTotalTimeoutMultiplier = 30;
	tmc.ReadTotalTimeoutConstant = 50;

	tmc.WriteTotalTimeoutMultiplier = 30;
	tmc.WriteTotalTimeoutConstant = 30;
	
	SetCommTimeouts(m_port, &tmc);
	SetCommState(m_port, &dcb);

	return true;
}


bool SerialPort::IsOpened(){
	if(m_port==NULL)
		return false;
	else
		return true;
}


bool SerialPort::SendData(unsigned char *data, unsigned char dataSize){
	if(!m_port)
		return false;

	unsigned long nbr=0;
	if(!WriteFile(m_port, data, dataSize, &nbr, 0) || nbr!=dataSize)
		return false;
	else
		return true;
}


bool SerialPort::ReceiveData(unsigned char *data, unsigned char *dataSize){
	if(!m_port || *dataSize==0)
		return false;

	unsigned long nbr=0;
	if(!ReadFile(m_port, &data[0], *dataSize, &nbr, NULL)){
		//PurgeComm(m_port, PURGE_RXCLEAR|PURGE_TXCLEAR);
		*dataSize = 0;
		return false;
	}
	
	if(nbr){
		*dataSize = (unsigned char)nbr;
		return true;
	}

	*dataSize = 0;
	return false;
}


bool SerialPort::Close(){
	if(m_port){
		CloseHandle(m_port);
		m_port = NULL;
		return true;
	}
	else {
		return false;
	}

}
