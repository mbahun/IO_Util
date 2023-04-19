#pragma once
#include "IIODevice.h"
#include "IProtocol.h"

class IODevice : public IIODevice {

	public:
		IODevice(IProtocol* protocol);
		virtual ~IODevice(void);
		bool SetOutput(unsigned char);
		unsigned char GetInput(bool reset=false);
		
	private:
		bool SendData(unsigned char *data, unsigned char dataSize);
		bool ReceiveData(unsigned char *data, unsigned char *dataSize);
		bool m_reqCheckInput;
};


//*******************************************************************************
//beeper class
//*******************************************************************************


class Beeper : public IIODevice {
	private:
		bool m_lastState=false;
	public:
		Beeper();
		virtual ~Beeper(void);
		bool SetOutput(unsigned char);
		unsigned char GetInput(bool reset = false);
};