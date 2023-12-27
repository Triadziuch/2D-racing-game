#pragma once

#define INPUT_DATA_BYTES 100

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

class SerialPort {
private:
	HANDLE handleToCOM;
	bool connected;
	COMSTAT status;
	DWORD errors;
	
public:
	SerialPort(char* portName);
	SerialPort(SerialPort* _obj);
	~SerialPort();

	int ReadSerialPort(char* buffer, unsigned int buf_size);
	bool WriteSerialPort(char* buffer, unsigned int buf_size);
	int getBufferSize();
	bool isConnected();
};