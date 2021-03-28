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
#include <LowPower.h>       // Library written by RocketScream 

#define aref_voltage 3.3  // we tie 3.3V to ARef 
const int soilMoistPin = A1;
int sensorPin = A0;   // Pin for measure Temp (TMP36)
float soilMoistRaw;
float soilMoistReal;
float FinalMoist;
float previousMoist;
float temperatureC;
float temperatureF;
const float alpha = 0.9; // const means value can't be changed, read only
float z;              // to store previous filtered value
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // lcd.setCursor(2,1); print 3 col from left and 2nd row. (col,row) from 0-15 and 0-1.
  // Print a message to the LCD.
  lcd.print("Hello, World!");
  delay(1000);
  
  // Print Soil Moisture to the LCD.
  lcd.print("Moisture:");
  lcd.setCursor(0,1); //1st col; 2nd row 
  lcd.print("T:");

  // set internal LED as output, used for indicating whether arduino has woken up
  pinMode(LED_BUILTIN, OUTPUT);

  // If you want to set the aref to something other than 5v
  analogReference(EXTERNAL);
}

void loop() {
  for(int i=0; i<=50; i++){
    // put your main code here, to run repeatedly:
    soilMoistRaw = analogRead(soilMoistPin)*(3.3/1024);
    delay(20);
  
    if (soilMoistRaw < 1.1) {
      soilMoistRaw = (10 * soilMoistRaw) - 1;
    }
    else if (soilMoistRaw < 1.3) {
      soilMoistReal = (25 * soilMoistRaw) - 17.5;
    }
    else if (soilMoistRaw < 1.82) {
      soilMoistReal = (48.08 * soilMoistRaw) - 47.5;
    }
    else if (soilMoistRaw < 2.2) {
      soilMoistReal = (26.32 * soilMoistRaw) - 7.89;
    }
    else {
      soilMoistReal = (62.5 * soilMoistRaw) - 87.5;
    }
    
    if (i==0){
      FinalMoist = soilMoistReal;       // initial data equal final data unfiltered.
      previousMoist = FinalMoist;
    }
    else{
      // do filtering, FinalMoist is the final value for moisture
      FinalMoist = (1-alpha)*soilMoistReal + alpha*previousMoist;
      previousMoist = FinalMoist;
    }
   
    Serial.println(soilMoistRaw);
    Serial.print("Water Content is: ");
    Serial.println(soilMoistReal);
    delay(100);
    //getting the voltage reading from the temperature sensor
    int reading = analogRead(sensorPin);  
    
    // converting that reading to voltage, for 3.3v arduino use 3.3
    float voltage = reading * aref_voltage / 1024.0; 
    
    if (i==0){
      float y = voltage;
      z = y;
    }
    else{
      // do filtering, y is the final value for voltage
      y = (1-alpha)*voltage + alpha*z;
      z = y;
    }
    
    // debug
    Serial.print("Analog "); Serial.println(reading); 
    // print out the voltage
    Serial.print(voltage, 4); Serial.print(" volts"); Serial.print("; Filtered "); Serial.println(y, 4);
     
    // now print out the temperature (TMP36 has offset of 0.5V), volt/degree from datasheet
    temperatureC = (y - 0.5) * 100 ;  //converting from 10 mv per degree with 500 mV offset
                                            //to degrees ((voltage - 500mV) times 100)
    Serial.print(temperatureC); Serial.println(" C");
  
    // now convert to Fahrenheit
    temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
    Serial.print(temperatureF); Serial.println(" F");
    delay(100);
  }
  
  // This is where you print the values onto LCD
  //print on 9th col from left and 1st row.
  lcd.setCursor(9,0); lcd.print(FinalMoist);
  lcd.setCursor(14,0); lcd.print("%");
  //3rd col; 2nd row
  lcd.setCursor(2,1); lcd.print(temperatureC); 
  lcd.setCursor(7,1); lcd.print("C;");
  lcd.setCursor(9,1); lcd.print(temperatureF);
  lcd.setCursor(14,1); lcd.print("F");
  
  // Flash LED to indicate Arduino going to sleep.
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);  
  
  
  // go to deep sleep for 56s, allows power optimization with slowest wake up time.
  for(int j=0; j<8; j++){
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}
