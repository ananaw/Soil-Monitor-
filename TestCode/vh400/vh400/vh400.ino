#include <LowPower.h>       // Library written by RocketScream 

const int soilMoistPin = A0;
float soilMoistRaw;
float soilMoistReal;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReference(EXTERNAL);
  
  // set internal LED as output, used for indicating whether arduino has woken up
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
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

  Serial.println(soilMoistRaw);

  Serial.print("Water Content is: ");
  Serial.println(soilMoistReal);
  delay(1000);
  
  digitalWrite(LED_BUILTIN, HIGH);  // Turn on LED to indicate Arduino going to sleep.
  delay(500);
  
  // go to deep sleep for 8s, allows power optimization with slowest wake up time.
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
}
