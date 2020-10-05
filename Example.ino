/*
 Name:		Test_FT897D.ino
 Created:	25.09.2020 7:06:13
 Author:	DtS
*/

#include <SoftwareSerial.h>
#include "dtsFT897D.h"

SoftwareSerial RadioPort(8,7);

dtsFT897D Radio(RadioPort);

void setup() {
	Serial.begin(115200);
	delay(250);
	Serial.println("Start...");
	Radio.Init(9600);
	Radio.SetMainFreq(14.25);
	Radio.ToggleVFO();
	Radio.SetRepeaterOffsetFreq(5.4321);
	Radio.SetCLAR(ON);
	Radio.SetCLARFreq(12.5);
	Radio.SetDCSMode(TDCS_Mode::DCS_ENCODER_ON);
	Radio.SetCTCSSToneFreq(88.0, 100.0);
	Radio.SetDCSCode(50, 9500);

	TTX_Status tx_status = Radio.ReadTXStatus();
	TRX_Status rx_status = Radio.ReadRXStatus();

}


void loop() {
}
