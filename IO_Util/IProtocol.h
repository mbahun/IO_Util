#pragma once
#include "StdAfx.h"

class IProtocol {
	public:
		virtual bool Open(CString address) = 0;
		virtual bool Close() = 0;
		virtual bool IsOpened() = 0;
		virtual bool SendData(unsigned char* data, unsigned char dataSize) = 0;
		virtual bool ReceiveData(unsigned char* data, unsigned char* dataSize) = 0;
};