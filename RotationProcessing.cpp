#include "RotationProcessing.h"

// LCD Initialization
void RotationProcessing::initLCDVariables(const float* _dystans, const float* _punkty, const float* _predkosc, const int* _zycia, const bool* _isEnd)
{
	this->dystans = _dystans;
	this->punkty = _punkty;
	this->predkosc = _predkosc;
	this->zycia = _zycia;
	this->isEnd = _isEnd;
}

// Constructors / Destructors
RotationProcessing::RotationProcessing()
{
	char* portC = new char[this->port.length() + 1];
	strcpy_s(portC, this->port.length() + 1, this->port.c_str());
	this->STM32 = new SerialPort(portC);
	delete portC;

	if (this->STM32->isConnected())
		cout << "Connected to " << port << endl;
	else
		cin.get();
}

RotationProcessing::~RotationProcessing()
{
	delete STM32;
}

// Update functions
void RotationProcessing::update()
{
	this->updateReceive();

	if (!this->message_iter)
		this->updateTransmitMessage();
	this->updateTransmit();
}

bool RotationProcessing::updateReceive()
{
	bool ret = false;
	if (this->STM32->isConnected())
		if (this->STM32->ReadSerialPort(this->inputData, INPUT_DATA_BYTES) != 0) {
			this->buffer.append(this->inputData);

			size_t len = buffer.find_first_of('\n');
			if (len != string::npos) {
				if (buffer[0] == '+' || buffer[0] == '-') {
					string next_buffer{};
					if (buffer.length() >= INPUT_DATA_BYTES)
						next_buffer = buffer.substr(len + 2, buffer.length());
					buffer.erase(len - 1, buffer.length());

					this->rotation_LR[0] = this->rotation_LR[1];
					this->rotation_FB[0] = this->rotation_FB[1];
					this->rotation_LR[1] = stod(buffer.substr(buffer.find_first_of(' ') + 1, buffer.length()));
					this->rotation_FB[1] = stod(buffer.substr(0, buffer.find_first_of(' ')));

					buffer = next_buffer;
					ret = true;
				}
				else
					buffer.clear();
			}
			memset(this->inputData, 0, sizeof this->inputData);
		}

	return ret;
}

void RotationProcessing::updateTransmitMessage()
{
	this->message = to_string(static_cast<int>(*this->dystans)) + ' ' +
					to_string(static_cast<int>(*this->punkty)) + ' ' +
					to_string(static_cast<int>(*this->predkosc)) + ' ' +
					to_string(*this->zycia) + ' ' + to_string(*this->isEnd) + '\n';
	cout << "DEBUG: Wysylany do STM C-string: \"" << this->message.substr(0, this->message.length() - 1) << "\\n\"" << endl;
}

void RotationProcessing::updateTransmit()
{
	// Aktualnie przesy³amy ci¹g w formacie "dystans punkty predkosc zycia isEnd\n"

	// Wysy³anie ci¹gu znaków
	if (this->STM32->isConnected())
		if (this->message_iter < this->message.length())
			this->STM32->WriteSerialPort(&this->message[this->message_iter++], 1);
		else
			this->message_iter = 0;
}
