/*
 Name:    Test_FT897D.ino
 Created: 25.09.2020 7:06:13
 Author:  DtS
*/
float ftFreq;
#include <SoftwareSerial.h>
#include "dtsFT897D.h"

SoftwareSerial RadioPort(2,3); // RX, TX

dtsFT897D Radio(RadioPort);

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println("Start...");
  Radio.Init(4800);
}


void loop() {
  
  ftFreq = Radio.GetFrequency();
  Serial.print("Freq = ");
  Serial.println(ftFreq);
  delay(1000);
}
