#pragma once
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_DATA_BYTES 100

class SerialPort 
{
private:
	// Private variables
	HANDLE handleToCOM;
	bool connected;
	COMSTAT status;
	DWORD errors;
	
public:
	// Constructors / Destructors
	SerialPort(char* portName);
	SerialPort(const SerialPort& _obj);
	~SerialPort();

	// Accessors / Mutators
	int getBufferSize()	 { return this->status.cbInQue; }
	bool isConnected()	 { return this->connected; }

	// Public functions
	int ReadSerialPort(char* buffer, unsigned int buf_size);
	bool WriteSerialPort(char* buffer, unsigned int buf_size);
};