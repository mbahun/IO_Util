#pragma once
#include "IProtocol.h"

class SerialPort : public IProtocol {
	private:
		HANDLE m_port;

	public:
		SerialPort(void);
		virtual ~SerialPort(void);

		bool Open(CString commPort);
		bool Close();
		bool IsOpened();
		bool SendData(unsigned char* data, unsigned char dataSize);
		bool ReceiveData(unsigned char* data, unsigned char* dataSize);

		unsigned char SearchSerialPorts(unsigned char fromPort);
		bool Open(unsigned char commPort, unsigned long baud);
};
