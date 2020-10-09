/*
 Name:    Test2.ino
 Created: 09.10.2020 
 Author:  UA6EM
*/
float ftFreq;
#include <SoftwareSerial.h>
#include "dtsFT897D.h"

SoftwareSerial RadioPort(2,3);

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
  delay(5000);
}
