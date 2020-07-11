#include <Arduino.h>
#include <RX8010SJ.h>

#define RX8010_I2C_ADDR 0x32

RX8010SJ::Adapter adapter = RX8010SJ::Adapter(RX8010_I2C_ADDR);
RX8010SJ::DateTime defaultDateTime = RX8010SJ::DateTime();

void setup() {
  defaultDateTime.seconds = 5;
  defaultDateTime.minutes = 25;
  defaultDateTime.hours = 15;

  Serial.begin(9600);
  adapter.initialise(true);
  adapter.writeDateTime(defaultDateTime);

  Serial.println("Initialised");
}

void loop() {
  RX8010SJ::DateTime dateTime = adapter.readDateTime();

  Serial.println("-------------------------------");
  Serial.print(dateTime.hours);
  Serial.print(":");
  Serial.print(dateTime.minutes);
  Serial.print(":");
  Serial.println(dateTime.seconds);
  Serial.println("-------------------------------");

	delay(1000);
}