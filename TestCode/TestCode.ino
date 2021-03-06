
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

 */

// include the library code:
#include <LiquidCrystal.h>


int sensorPin = 1;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {

  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

}

void loop() {


  int sensorValue = analogRead(A0);
  Serial.print("Moisture Prob: "); Serial.println(sensorValue);


 //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin);  
 

 float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 // print out the voltage
 Serial.print(voltage); Serial.println(" volts");
 
 // now print out the temperature (TMP36 has offset of 0.5V), volt/degree from datasheet
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 Serial.print(temperatureC); Serial.println(" degrees C");
 
 // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 Serial.print(temperatureF); Serial.println(" degrees F");

 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Moisture: ");
 lcd.print(sensorValue);
 lcd.setCursor(0,1);
 lcd.print("Temprature: ");
 lcd.print(temperatureF);
 lcd.print(" F");
 
  
 delay(1000);

    
}
