
const int soilMoistPin = A1;
float soilMoistRaw;
float soilMoistReal;
const int runningCount = 16;
float runningAvgBuffer[runningCount];
int nextRunning;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReference(EXTERNAL);

}

void loop() {
  // put your main code here, to run repeatedly:
  soilMoistRaw = analogRead(soilMoistPin)*(3.3/1024);
  delay(20);

  if (soilMoistRaw < 1.1) {
    soilMoistReal = (10 * soilMoistRaw) - 1;
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

  runningAvgBuffer[nextRunning++] = soilMoistReal;
  if(nextRunning >= runningCount)
  {
    nextRunning = 0;
  }

  float soilMoistAvg = 0;

  for(int i = 0; i < runningCount; ++i)
  {
    soilMoistAvg += runningAvgBuffer[i];
  }

  soilMoistAvg /= runningCount;

  Serial.println(soilMoistAvg);
  delay(1000);
}
