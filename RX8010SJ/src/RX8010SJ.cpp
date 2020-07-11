#include "RX8010SJ.h"

namespace RX8010SJ {

    Adapter::Adapter(byte i2cSlaveAddr) {
        i2cAddress = i2cSlaveAddr;
    }

    Adapter::~Adapter() {}

	void Adapter::initialise(bool reset) {
		Wire.begin();
  		delay(40);
	}

    void Adapter::readDateTime() {
		byte secondsBin = writeToModule(RX8010_SEC);
		byte minutesBin = writeToModule(RX8010_MIN);
		byte hoursBin = writeToModule(RX8010_HOUR);

		byte seconds = getValueFromBinary(secondsBin, 6, 40);
		seconds += getValueFromBinary(secondsBin, 5, 20);
		seconds += getValueFromBinary(secondsBin, 4, 10);
		seconds += getValueFromBinary(secondsBin, 3, 8);
		seconds += getValueFromBinary(secondsBin, 2, 4);
		seconds += getValueFromBinary(secondsBin, 1, 2);
		seconds += getValueFromBinary(secondsBin, 0, 1);

		byte minutes = getValueFromBinary(minutesBin, 6, 40);
		minutes += getValueFromBinary(minutesBin, 5, 20);
		minutes += getValueFromBinary(minutesBin, 4, 10);
		minutes += getValueFromBinary(minutesBin, 3, 8);
		minutes += getValueFromBinary(minutesBin, 2, 4);
		minutes += getValueFromBinary(minutesBin, 1, 2);
		minutes += getValueFromBinary(minutesBin, 0, 1);

		byte hours = getValueFromBinary(hoursBin, 5, 20);
		hours += getValueFromBinary(hoursBin, 4, 10);
		hours += getValueFromBinary(hoursBin, 3, 8);
		hours += getValueFromBinary(hoursBin, 2, 4);
		hours += getValueFromBinary(hoursBin, 1, 2);
		hours += getValueFromBinary(hoursBin, 0, 1);

		Serial.println("-------------------------------");
		Serial.print(hours);
		Serial.print(":");
		Serial.print(minutes);
		Serial.print(":");
		Serial.println(seconds);
		Serial.println("-------------------------------");
    }

	byte Adapter::writeToModule(byte data) {
		Wire.beginTransmission(i2cAddress);
		Wire.write(data);
		Wire.endTransmission();
		Wire.requestFrom(i2cAddress, 1);

		if (Wire.available()) {
			return Wire.read();
		}

		return -1;
	}

	byte Adapter::getValueFromBinary(byte binary, byte pos, byte val) {
		return ((binary >> pos) & 1) == 1 ? val : 0;
	}

}