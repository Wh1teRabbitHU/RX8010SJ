#include "RX8010SJ.h"

namespace RX8010SJ {

    Adapter::Adapter(byte i2cSlaveAddr) {
        i2cAddress = i2cSlaveAddr;
    }

    Adapter::~Adapter() {}

	bool Adapter::initAdapter() {
		Wire.begin();
  		delay(40);

		return initModule();
	}

	bool Adapter::initModule() {
		byte flagValue = readFromModule(RX8010_FLAG);
		byte vlf = getValueFromBinary(flagValue, 1, 1);

		// It has power issue and needs a reinitialisation
		if (vlf == 1) {
			while (vlf == 1) {
				flagValue = setBinary(flagValue, 1, 0);
				writeToModule(RX8010_FLAG, vlf);
				delay(10);
				flagValue = readFromModule(RX8010_FLAG);
				vlf = getValueFromBinary(flagValue, 1, 1);
			}

			resetModule();

			return true;
		}

		return false;
	}

	void Adapter::resetModule() {
		writeToModule(RX8010_RESV17, RX8010_ADDR17_DEF_VAL);
		writeToModule(RX8010_RESV30, RX8010_ADDR30_DEF_VAL);
		writeToModule(RX8010_RESV31, RX8010_ADDR31_DEF_VAL);
		writeToModule(RX8010_IRQ, RX8010_IRQ_DEF_VAL);

		writeFlag(RX8010_EXT, 4, 0);  // TE bit (4) to zero
		writeFlag(RX8010_FLAG, 1, 0); // VLF bit (1) to zero

		writeToModule(RX8010_CTRL, RX8010_CTRL_DEF_VAL);
	}

    DateTime Adapter::readDateTime() {
		DateTime dateTime;
		byte secondsBin = readFromModule(RX8010_SEC);
		byte minutesBin = readFromModule(RX8010_MIN);
		byte hoursBin = readFromModule(RX8010_HOUR);

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

		dateTime.seconds = seconds;
		dateTime.minutes = minutes;
		dateTime.hours = hours;

		return dateTime;
    }

	void Adapter::writeDateTime(DateTime dateTime) {
		byte seconds = dateTime.seconds % 10;
		byte minutes = dateTime.minutes % 10;
		byte hours = dateTime.hours % 10;

		seconds = setFortyBinary(seconds, dateTime.seconds);
		seconds = setTwentyBinary(seconds, dateTime.seconds);
		seconds = setTenBinary(seconds, dateTime.seconds);

		minutes = setFortyBinary(minutes, dateTime.minutes);
		minutes = setTwentyBinary(minutes, dateTime.minutes);
		minutes = setTenBinary(minutes, dateTime.minutes);

		hours = setTwentyBinary(hours, dateTime.hours);
		hours = setTenBinary(hours, dateTime.hours);

		writeToModule(RX8010_SEC, seconds);
		writeToModule(RX8010_MIN, minutes);
		writeToModule(RX8010_HOUR, hours);
	}

	byte Adapter::readFromModule(byte address) {
		Wire.beginTransmission(i2cAddress);
		Wire.write(address);
		Wire.endTransmission();
		Wire.requestFrom(i2cAddress, 1);

		if (Wire.available()) {
			return Wire.read();
		}

		return -1;
	}

	void Adapter::writeToModule(byte address, byte data) {
		Wire.beginTransmission(i2cAddress);
		Wire.write(address);
		Wire.write(data);
		Wire.endTransmission();
	}

	void Adapter::writeFlag(byte address, byte pos, byte value) {
		byte addressValue = readFromModule(address);
		addressValue = setBinary(addressValue, pos, value);
		writeToModule(address, addressValue);
	}

	byte Adapter::getValueFromBinary(byte binary, byte pos, byte val) {
		return ((binary >> pos) & 1) == 1 ? val : 0;
	}

	byte Adapter::setFortyBinary(byte binary, byte val) {
		if (val >= 40) {
			return setBinary(binary, 6, 1);
		}	

		return setBinary(binary, 6, 0);
	}

	byte Adapter::setTwentyBinary(byte binary, byte val) {
		if (val >= 20 && val < 40) {
			return setBinary(binary, 5, 1);
		}	

		return setBinary(binary, 5, 0);
	}

	byte Adapter::setTenBinary(byte binary, byte val) {
		if ((val >= 10 && val < 20) ||
			(val >= 30 && val < 40) ||
			(val >= 50 && val < 60)) {

			return setBinary(binary, 4, 1);
		}	

		return setBinary(binary, 4, 0);
	}

	byte Adapter::setBinary(byte binary, byte pos, byte flagVal) {
		if (flagVal == 1) {
			return binary | (1 << pos);
		}

		return binary & (~(1 << pos));
	}

}