# RX8010SJ

- [RX8010SJ](#rx8010sj)
- [Introduction](#introduction)
	- [How to use it?](#how-to-use-it)
	- [API](#api)
		- [Adapter - Methods](#adapter---methods)
		- [DateTime - Attributes](#datetime---attributes)

# Introduction

This is a library to handle Seiko's RX8010SJ RTC module with an arduino. Original datasheet can be found [here](files/RX8010SJ.pdf). To set up your module, you only need to power up and use the i2c protocol to communicate with it. You have to provide the slave address to the constructor to start the communication. As many other i2c devices, this one also needs pull up resistors on the communication ports. (I'm using 5K, but 10K or 1K should be also good) The library is using the built-in "Wire" library to send and receive messages.

## How to use it?

The current version only has methods for setting and retrieving date-time, implementing all the other features is still in progress. With the following code, you can get the current time at every second:

```c
#include <RX8010SJ.h>

#define RX8010_I2C_ADDR 0x32

const char * dayOfWeekStrings[] = {
  	"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
};

RX8010SJ::Adapter adapter = RX8010SJ::Adapter(RX8010_I2C_ADDR);
RX8010SJ::DateTime defaultDateTime = RX8010SJ::DateTime();

void setup() {
	defaultDateTime.second = 5;
	defaultDateTime.minute = 25;
	defaultDateTime.hour = 15;
	defaultDateTime.dayOfWeek = 5;
	defaultDateTime.dayOfMonth = 18;
	defaultDateTime.month = 7;
	defaultDateTime.year = 20;

	Serial.begin(9600);
	bool reset = adapter.initAdapter();

	if (reset) {
		Serial.println("The adapter has been reset!");
		adapter.writeDateTime(defaultDateTime);
	}

	Serial.println("Initialised");
}

void loop() {
	RX8010SJ::DateTime dateTime = adapter.readDateTime();

	Serial.println("-------------------------------");
	Serial.print("20");
	Serial.print(dateTime.year);
	Serial.print("-");
	Serial.print(dateTime.month);
	Serial.print("-");
	Serial.print(dateTime.dayOfMonth);
	Serial.print(" (");
	Serial.print(dayOfWeekStrings[dateTime.dayOfWeek]);
	Serial.print(") ");
	Serial.print(dateTime.hour);
	Serial.print(":");
	Serial.print(dateTime.minute);
	Serial.print(":");
	Serial.println(dateTime.second);
	Serial.println("-------------------------------");

	delay(1000);
}
```

## API

There are two main objects that can be used in this library: The Adapter class and the DateTime struct. The adapter instance will take care of the communications and the DateTime struct is storing the time data.

### Adapter - Methods

- __void Constructor(byte i2cSlaveAddr)__: When you create an instance, you have to pass the slave address as a parameter, so the initialisation can open an i2c connection.
- __bool initAdapter(void)__: You have to call this method before you can start using the library, because it will setup the i2c communication library. It will also call the initModule method, so you don't have to do it separately and it returns with the initModule's return value.
- __bool initModule(void)__: This method will initialise the RX8010SJ if needed. Firstly it checks the VLF flag and if the oscillator had issue, then it will wait arbitrary amount of time until the module restart itself and then it call the resetModule method to clear any invalid state. In this case any predefined time values or setting flags will be reset to default. This method will return with a bool depending on the state of the VLF flag. If the module needed to execute a software reset, then it returns true. (recommended to check the outcome and if needed, setup the module with the desired values afterwards)
- __DateTime readDateTime(void)__: This method will read the module and returns with the current state of the clock. DateTime is a struct which is holding date and time related values. For more details about it's attributes, check the DateTime documentation bellow.
- __void writeDateTime(DateTime dateTime)__: You can set or update the time with calling this method. The input parameter is a custom DateTime struct instance. For more details about it's attributes, check the DateTime documentation bellow.
- __void setFCTCounter(uint16_t multiplier, byte frequency)__: (TODO: Function implemented, documentation needed)
- __uint16_t getFCTCounter()__: (TODO: Function implemented, documentation needed)
- __void setFCTOutput(byte pin)__: (TODO: Function implemented, documentation needed)
- __void enableFCT()__: (TODO: Function implemented, documentation needed)
- __void disableFCT()__: (TODO: Function implemented, documentation needed)
- __bool checkFCT()__: (TODO: Function implemented, documentation needed)
- __void setAlarm(DateTime time, byte mode)__: (TODO: Function implemented, documentation needed)
- __void enableAlarm()__: (TODO: Function implemented, documentation needed)
- __void disableAlarm()__: (TODO: Function implemented, documentation needed)
- __bool checkAlarm()__: (TODO: Function implemented, documentation needed)
- __void setTUIMode(bool minute)__: (TODO: Function implemented, documentation needed)
- __void enableTUI()__: (TODO: Function implemented, documentation needed)
- __void disableTUI()__: (TODO: Function implemented, documentation needed)
- __bool checkTUI()__: (TODO: Function implemented, documentation needed)
- __void enableFOUT(byte frequency, byte pin)__: (TODO: Function implemented, documentation needed)
- __void disableFOUT()__: (TODO: Function implemented, documentation needed)

### DateTime - Attributes

- __byte second__: A number type attribute which is storing the second part of the time. Valid values are: 0-59
- __byte minute__: A number type attribute which is storing the minute part of the time. Valid values are: 0-59
- __byte hour__: A number type attribute which is storing the hour part of the time. Valid values are: 0-23
- __byte dayOfWeek__: A number type attribute which is storing the day of the week. You have to update this value, when you are changing the date, because the module handles those two informations independently. Valid values are: 0-6
- __byte dayOfMonth__: A number type attribute which is storing the day of the selected month. Depending on the year value it is capable of calculating and using leap days. Valid values are: 0-31
- __byte month__: A number type attribute which is storing the month part of the date. This attribute is starts from 1. Valid values are: 1-12
- __byte year__: A number type attribute which is storing the year part of the date. This value is only 2 digit long and every four year count as a leap year. Valid values are: 0-99