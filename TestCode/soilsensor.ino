// To use Adafruit seesaw library, install the Adafruit Seesaw Library by pressing Ctrl+Shift+I, search for Adafruit Seesaw Library, and install.  
#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

void setup() {
  Serial.begin(115200);

  Serial.println("seesaw Soil Sensor example!");
  
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
}

void loop() {
  float tempC = ss.getTemp();
  float tempF = (tempC * 9.0 / 5.0) + 32.0;
 
  uint16_t capread = ss.touchRead(0);

  Serial.print("Temperature: "); Serial.print(tempC); Serial.print("*C; "); Serial.print(tempF); Serial.println(" degrees F");
  Serial.print("Capacitive: "); Serial.println(capread);
  delay(1000);
}
