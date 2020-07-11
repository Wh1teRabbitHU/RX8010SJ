#include <Arduino.h>
#include <Wire.h>

#define RX8010_ADDR    0x32

#define RX8010_SEC     0x10
#define RX8010_MIN     0x11
#define RX8010_HOUR    0x12
#define RX8010_WDAY    0x13
#define RX8010_MDAY    0x14
#define RX8010_MONTH   0x15
#define RX8010_YEAR    0x16
#define RX8010_RESV17  0x17
#define RX8010_ALMIN   0x18
#define RX8010_ALHOUR  0x19
#define RX8010_ALWDAY  0x1A
#define RX8010_TCOUNT0 0x1B
#define RX8010_TCOUNT1 0x1C
#define RX8010_EXT     0x1D
#define RX8010_FLAG    0x1E
#define RX8010_CTRL    0x1F
/* 0x20 to 0x2F are user registers */
#define RX8010_RESV30  0x30
#define RX8010_RESV31  0x31
#define RX8010_IRQ     0x32

#define RX8010_EXT_WADA  BIT(3)

#define RX8010_FLAG_VLF  BIT(1)
#define RX8010_FLAG_AF   BIT(3)
#define RX8010_FLAG_TF   BIT(4)
#define RX8010_FLAG_UF   BIT(5)

#define RX8010_CTRL_AIE  BIT(3)
#define RX8010_CTRL_UIE  BIT(5)
#define RX8010_CTRL_STOP BIT(6)
#define RX8010_CTRL_TEST BIT(7)

#define RX8010_ALARM_AE  BIT(7)

byte writeToModule(byte data) {
  Wire.beginTransmission(RX8010_ADDR); // transmit to device #44 (0x2c)
  Wire.write(data);            // sends instruction byte
  Wire.endTransmission();
  Wire.requestFrom(RX8010_ADDR, 1);

  if (Wire.available()) {
  	return Wire.read();
  }

  return -1;
}

byte getValueFromBinary(byte binary, byte pos, byte val) {
  return ((binary >> pos) & 1) == 1 ? val : 0;
}

void readTime() {
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

void setup() {
  Serial.begin(9600);
  Wire.begin();

  delay(40);
  Serial.println("Initialised");
}

void loop() {
  readTime();

	delay(1000);
}