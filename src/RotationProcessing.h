#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include "SerialPort.h"

using namespace std;

class RotationProcessing {
private:
	// Private variables
	char		 inputData[INPUT_DATA_BYTES];
	string		 buffer{};
	double		 rotation_LR[2]{}, rotation_FB[2]{};
	string		 port = "COM4";
	SerialPort	 *STM32;
	size_t		 message_iter{};
	string		 message{};

	// LCD Variables
	const float* dystans, * punkty, * predkosc;
	const int* zycia;
	const bool* isEnd;
	
public:
	// LCD Initialization
	void initLCDVariables(const float* _dystans, const float* _punkty, const float* _predkosc, const int* _zycia, const bool* _isEnd);

	// Constructors / Destructors
	RotationProcessing();
	~RotationProcessing();

	// Update functions
	void update();
	bool updateReceive();
	void updateTransmitMessage();
	void updateTransmit();
	

	// Accessors / Mutators
	bool getIsConnected()	 { return this->STM32->isConnected(); };

	const double getNewLR()  { return this->rotation_LR[1]; }
	const double getNewFB()  { return this->rotation_FB[1]; }
	const double getOldLR()  { return this->rotation_LR[0]; }
	const double getOldFB()  { return this->rotation_FB[0]; }
	const double getDiffLR() { return this->rotation_LR[1] - this->rotation_LR[0]; }
	const double getDiffFB() { return this->rotation_FB[1] - this->rotation_FB[0]; }
};