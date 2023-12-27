#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include "SerialPort.h"

using namespace std;

class RotationProcessing {
private:
	char		 inputData[INPUT_DATA_BYTES];
	string		 buffer{};
	double		 rotation_LR[2]{}, rotation_FB[2]{};
	string		 port = "COM4";
	SerialPort	 *STM32;
	
public:

	RotationProcessing();
	~RotationProcessing();

	bool getIsConnected();
	const double getNewLR()  { return this->rotation_LR[1]; }
	const double getNewFB()  { return this->rotation_FB[1]; }
	const double getOldLR()  { return this->rotation_LR[0]; }
	const double getOldFB()  { return this->rotation_FB[0]; }
	const double getDiffLR() { return this->rotation_LR[1] - this->rotation_LR[0]; }
	const double getDiffFB() { return this->rotation_FB[1] - this->rotation_FB[0]; }
	bool update();
};