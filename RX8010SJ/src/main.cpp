#include <Arduino.h>
#include <RX8010SJ.h>

#define RX8010_I2C_ADDR 0x32

RX8010SJ::Adapter adapter = RX8010SJ::Adapter(RX8010_I2C_ADDR);

void setup() {
  Serial.begin(9600);
  adapter.initialise(true);
  Serial.println("Initialised");
}

void loop() {
  adapter.readDateTime();

	delay(1000);
}