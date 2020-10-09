/*
 Name:		Test_FT897D.ino
 Created:	25.09.2020 7:06:13
 Author:	DtS
*/
float ftFreq;
#include <SoftwareSerial.h>
#include "dtsFT897D.h"

SoftwareSerial RadioPort(8,7);

dtsFT897D Radio(RadioPort);

void setup() {
	Serial.begin(115200);
	delay(250);
	Serial.println("Start...");
	Radio.Init(4800);
}


void loop() {
	
	ftFreq = Radio.GetFrequency();
	Serial.println("Freq = ",ftFreq);
	delay(5000);
}
