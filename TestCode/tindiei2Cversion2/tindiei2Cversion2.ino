// To use this code, I installed the I2CSoilMoistureSensor Library provided by Ingo Fischer in Arduino.
// Also, for the sensor to work, have to connect two pull-up resistors from SDA(A4) and SCK(A13) to +5V each. Use 4.7k resistors.
#include <I2CSoilMoistureSensor.h>
#include <Wire.h>

I2CSoilMoistureSensor sensor;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  sensor.begin(); // reset sensor
  delay(1000); // give some time to boot up
  Serial.print("I2C Soil Moisture Sensor Address: ");
  Serial.println(sensor.getAddress(),HEX);
  Serial.print("Sensor Firmware version: ");
  Serial.println(sensor.getVersion(),HEX);
  Serial.println();
}

void loop() {
  while (sensor.isBusy()) delay(50); // available since FW 2.3
  Serial.print("Soil Moisture Capacitance: ");
  Serial.println(sensor.getCapacitance()); //read capacitance register
  Serial.print(", Temperature: ");
  Serial.println(sensor.getTemperature()/(float)10); //temperature register
  sensor.sleep(); // available since FW 2.3
}
