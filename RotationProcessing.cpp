#include "RotationProcessing.h"

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

bool RotationProcessing::getIsConnected()
{
	return this->STM32->isConnected();
}

bool RotationProcessing::update()
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
