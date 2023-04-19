#include "StdAfx.h"
#include "IODevice.h"


IODevice::IODevice(IProtocol* protocol) : IIODevice(protocol) {
	m_reqCheckInput = true;
}


IODevice::~IODevice(void){
}


bool IODevice::SetOutput(unsigned char output){
	unsigned char data[255];
	unsigned char size=8;

	data[0]=0x02;
	data[1]=0xFF;
	data[2]=0x02;
	data[3]=0x73;
	data[4]=output;
	data[5]=data[2] ^ data[3] ^ data[4];
	data[6]=0x03;
	data[7]=0x00;

	return SendData(data, size);
	//data will be received in GetInput()!
}


unsigned char IODevice::GetInput(bool reset){
	unsigned char data[8], size=8;
	RtlZeroMemory(data, size);
	data[0]=0x02;
	data[1]=0xFF;
	data[2]=0x02;
	data[3]=0x69; //'i'
	data[4]=0x00; //anything
	data[5]=data[2] ^ data[3] ^ data[4];
	data[6]=0x03;
	data[7]=0x00;

	if(reset)
		m_reqCheckInput = true;

	if(m_reqCheckInput){
		SendData(data, 8);
		m_reqCheckInput = false;
	}

	size = 8;
	RtlZeroMemory(data, size);
	if(ReceiveData(data, &size)){
		if(data[0]=='i'){
			m_reqCheckInput = true;
			return data[1];
		}
		else{
			return 0;
		}
	}

	return 0;
}


bool IODevice::SendData(unsigned char *data, unsigned char dataSize){
	return m_protocol->SendData(data, dataSize);
}


bool IODevice::ReceiveData(unsigned char *data, unsigned char *dataSize){
	if(m_protocol->ReceiveData(data, dataSize)){
		if((data[2] ^ data[3] ^ data[4]) == data[5]){
			data[0] = data[3]; //command
			data[1] = data[4]; //input pins
			RtlZeroMemory(&data[2], (*dataSize)-2);
			*dataSize = 2;
			return true;
		}
	}
	return false;
}


//*******************************************************************************
//beeper class
//*******************************************************************************


Beeper::Beeper() : IIODevice(NULL) {
	m_lastState = false;
}


Beeper::~Beeper(void) {

}


bool Beeper::SetOutput(unsigned char state) {
	bool newState = state ? true : false;

	if (newState != m_lastState) {
		m_lastState = newState;

		newState ? Beep(2500, 150) : Beep(1500, 150);
	}

	return true;
}


unsigned char Beeper::GetInput(bool reset) {
	return 0;
}

