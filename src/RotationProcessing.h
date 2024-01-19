#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "SerialPort.h"

using namespace std;

class RotationProcessing {
private:
	// Private variables
	char		 inputData[INPUT_DATA_BYTES];
	string		 buffer{};
	double		 rotation_LR[2]{}, rotation_FB[2]{};
	bool		 using_keyboard_input;
	string		 port = "COM4";
	SerialPort	 *STM32;
	size_t		 message_iter{};
	string		 message{};

	// LCD Variables
	const float* dystans, * punkty, * predkosc;
	const int* zycia;
	const bool* isPlaying;

	// Private functions
	void readCOMfromfile();
	
public:
	// LCD Initialization
	void initLCDVariables(const float* _dystans, const float* _punkty, const float* _predkosc, const int* _zycia, const bool* _isPlaying);

	// Constructors / Destructors
	RotationProcessing();
	~RotationProcessing();

	// Update functions
	void update();
	bool updateReceive();
	void updateTransmitMessage();
	void updateTransmit();
	void updatePollEvents();
	

	// Accessors / Mutators
	bool getIsConnected()	 { return this->STM32->isConnected(); };
	bool getIsMouse()		 { return this->using_keyboard_input; }

	const double getNewLR()  { return this->rotation_LR[1]; }
	const double getNewFB()  { return this->rotation_FB[1]; }
	const double getOldLR()  { return this->rotation_LR[0]; }
	const double getOldFB()  { return this->rotation_FB[0]; }
	const double getDiffLR() { return this->rotation_LR[1] - this->rotation_LR[0]; }
	const double getDiffFB() { return this->rotation_FB[1] - this->rotation_FB[0]; }
};