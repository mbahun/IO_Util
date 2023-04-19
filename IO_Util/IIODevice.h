#pragma once
#include "IProtocol.h"

class IIODevice {
	protected:
		IIODevice(IProtocol* protocol) { m_protocol = protocol; }
		IProtocol* m_protocol;

	public:
		virtual bool SetOutput(unsigned char) = 0;
		virtual unsigned char GetInput(bool reset=false) = 0;
};

