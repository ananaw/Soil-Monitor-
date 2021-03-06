/*  
  The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * 
 * STEMMA soil sensor
 * red wire to 3-5V,
 * black wire to GND
 * SCL Green wire to A5
 * SDA white wire to A4
 */

// include the library code:
#include <LiquidCrystal.h>
#include "Adafruit_seesaw.h"
#include <LowPower.h>       // Library written by RocketScream 

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

Adafruit_seesaw ss;
const int AirVal = 321;   
const int WaterVal = 621;
int intervals = (WaterVal - AirVal)/3;   
int soilMoistureVal = 0;
  
void setup() {
  Serial.begin(9600);

  Serial.println("seesaw Soil Sensor example!");
  
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // lcd.setCursor(2,1); print 3 col from left and 2nd row. (col,row) from 0-15 and 0-1.
  // Print a message to the LCD.
  lcd.print("Hello, World!");

  // set internal LED as output, used for indicating whether arduino has woken up
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
  float tempC = ss.getTemp();
  float tempF = (tempC * 9.0 / 5.0) + 32.0;
 
  uint16_t capread = ss.touchRead(0);

  // turn off LED
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.print("Temperature: "); Serial.print(tempC); Serial.print("*C; "); Serial.print(tempF); Serial.println(" degrees F");
  Serial.print("Capacitive: "); Serial.println(capread);

  // translate the air - water range to percentage using map() function
  int percentage = map(capread, AirVal, WaterVal, 0, 100);
  Serial.print(percentage); Serial.println("%");
  if(percentage >= 75) { 
    Serial.println("Wet"); } 
  else if(35 <= (percentage) && (percentage) < 75)
  {
  Serial.println("Moist");
  }
  else if(percentage < 35)
  {
  Serial.println("Dry");
  }
  
  delay(1000);
  
  digitalWrite(LED_BUILTIN, HIGH);  // Turn on LED to indicate Arduino going to sleep.
  delay(500);
  
  // go to deep sleep for 8s, allows power optimization with slowest wake up time.
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
}
