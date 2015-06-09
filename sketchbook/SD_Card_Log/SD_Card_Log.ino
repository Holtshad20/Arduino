#include <SD.h>

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card ... ");
  
  /* inicializar SD Card */
  if (!SD.begin(10)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initilized.");
}


void loop() {
  String dataString = "";
  
  int sensor = analogRead(0);
  dataString = String(sensor);
  
  File dataFile = SD.open("log.txt", FILE_WRITE);
  
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  else {
    Serial.println("Error opening log.txt");
  }
  delay(10000);
}

