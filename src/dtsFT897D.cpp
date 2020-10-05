﻿#include "dtsFT897D.h"

void dtsFT897D::ClearCmdBuffer(void)
{
	memset(&FCommand, 0, COMMAND_SIZE);
}

void dtsFT897D::SendCommand(void) const
{
		FPort.write((uint8_t *)(&FCommand), COMMAND_SIZE);
}

void dtsFT897D::Init(const uint32_t ABaudRate)
{
	FPort.begin(ABaudRate);    // настроить скорость Software порта. По умолчанию == 9600
	delay(20);
	SetMode();                 // после инициализации переключить рабочий режим на ключ (по умолчанию)
}

void dtsFT897D::SetLock(const bool AValue)
{
	ClearCmdBuffer();
	FCommand.Command = (AValue == ON) ? CMD_LOCK_ON : CMD_LOCK_OFF;
	SendCommand();
}

void dtsFT897D::SetPTT(const bool AValue)
{
	ClearCmdBuffer();
	FCommand.Command = (AValue == ON) ? CMD_PTT_ON : CMD_PTT_OFF;
	SendCommand();

}

void dtsFT897D::SetMode(const TOperatingMode AMode)
{
	ClearCmdBuffer();
	FCommand.Byte0 = static_cast<uint8_t>(AMode);
	FCommand.Command = CMD_SET_OPERATING_MODE;
	SendCommand();
}

void dtsFT897D::SetCLAR(const bool AValue)
{
	ClearCmdBuffer();
	FCommand.Command = (AValue == ON) ? CMD_CLAR_ON : CMD_CLAR_OFF;
	SendCommand();
}

void dtsFT897D::SetCLARFreq(float AClarFreq)
{
	ClearCmdBuffer();
	if (AClarFreq < 0) {
		FCommand.Byte0 = 1;
		AClarFreq = -AClarFreq;
	}

	if (AClarFreq >= 100.00) AClarFreq = 99.99;

	const char* ptr = Freq2String(AClarFreq, 2, 4);

	StringToBCD(ptr, &FCommand.Byte2, 2);

	FCommand.Command = CMD_CLAR_SETFREQ;

	SendCommand();
}

void dtsFT897D::SetSplit(const bool AValue)
{
	ClearCmdBuffer();
	FCommand.Command = (AValue == ON) ? CMD_SPLIT_ON : CMD_SPLIT_OFF;
	SendCommand();
}

void dtsFT897D::SetRepeaterOffsetDir(const TRepeaterOffset AValue)
{
	ClearCmdBuffer();
	FCommand.Byte0 = static_cast<uint8_t>(AValue);
	FCommand.Command = CMD_SET_REPEATER_OFFSET;
	SendCommand();
}

void dtsFT897D::SetRepeaterOffsetFreq(float ARepFreq)
{
	if (ARepFreq < 0) ARepFreq = -ARepFreq;
	if (ARepFreq >= 100.0) ARepFreq = 99.99;
	const char* ptr = Freq2String(ARepFreq, 2, 8);

	StringToBCD(ptr, &FCommand.Byte0, 4);

	FCommand.Command = CMD_SET_REPEATER_OFFSET_FREQ;

	SendCommand();

}

void dtsFT897D::SetMainFreq(float AMainFreq)
{
	ClearCmdBuffer();

	if (AMainFreq < 0) AMainFreq = -AMainFreq;
	const char* ptr = Freq2String(AMainFreq, 3, 8);

	StringToBCD(ptr, &FCommand.Byte0, 4);

	FCommand.Command = CMD_SET_MAIN_FREQ;

	SendCommand();
}

void dtsFT897D::ToggleVFO(void)
{
	ClearCmdBuffer();
	FCommand.Command = CMD_SWITCH_VFO;
	SendCommand();
}

void dtsFT897D::SetDCSMode(const TDCS_Mode AMode)
{
	ClearCmdBuffer();
	FCommand.Byte0 = static_cast<uint8_t>(AMode);
	FCommand.Command = CMD_SET_DCS_MODE;
	SendCommand();
}

void dtsFT897D::SetCTCSSToneFreq(float ATXFreq, float ARXFreq)
{
	if (ATXFreq < 0) ATXFreq = -ATXFreq;
	if (ARXFreq < 0) ARXFreq = -ARXFreq;

	ClearCmdBuffer();

	const char* ptr = Freq2String(ATXFreq, 3, 4);
	StringToBCD(ptr, &FCommand.Byte0, 2);

	ptr = Freq2String(ARXFreq, 3, 4);
	StringToBCD(ptr, &FCommand.Byte2, 2);

	FCommand.Command = CMD_SET_CTCSS_TONE_FREQ;

	SendCommand();

}

void dtsFT897D::SetDCSCode(const uint16_t ATXCode, const uint16_t ARXCode)
{
	ClearCmdBuffer();
	
	const char* ptr = Freq2String(ATXCode / 100.0, 2, 4);
	StringToBCD(ptr, &FCommand.Byte0, 2);

	ptr = Freq2String(ARXCode / 100.0, 2, 4);
	StringToBCD(ptr, &FCommand.Byte2, 2);

	FCommand.Command = CMD_SET_DCS_CODE;

	SendCommand();
}

TRX_Status dtsFT897D::ReadRXStatus()
{
	FlushPort();
	ClearCmdBuffer();
	FCommand.Command = CMD_READ_RX_STATUS;
	SendCommand();

	TRX_Status result;
	uint8_t value = ReadByteFromPort(1000);

	memcpy(&result, &value, 1);

	return result;
}

TTX_Status dtsFT897D::ReadTXStatus()
{
	FlushPort();
	ClearCmdBuffer();

	FCommand.Command = CMD_READ_TX_STATUS;
	SendCommand();

	TTX_Status result;
	uint8_t value = ReadByteFromPort(1000);

	memcpy(&result, &value, 1);

	return result;
}

const char* dtsFT897D::Freq2String(const float AFreq, const uint8_t AIntDigits, const uint8_t ALength)
{
	static char outbuf[10];
	memset(outbuf, '0', ALength);
	outbuf[ALength] = 0x00;

	uint32_t Multiplier = 1;
	for (uint8_t i = 0; i < (ALength-AIntDigits); ++i) { Multiplier *= 10; };

	uint32_t Freq = AFreq * Multiplier;

	char* ptr = &outbuf[AIntDigits];
	if (AFreq >= 1.0) ptr--;
	if (AFreq >= 10.0) ptr--;
	if (AFreq >= 100.0) ptr--;

	ultoa(Freq, ptr, 10);

	return outbuf;
}

void dtsFT897D::StringToBCD(const char* AFrom, uint8_t* ATo, const uint8_t ALen)
{
	uint8_t idx = 0;
	for (uint8_t i = 0; i < ALen; ++i) {
		ATo[i] = ((AFrom[idx] - '0') << 4) | (AFrom[idx + 1] - '0');
		idx += 2;
	}
}

uint8_t dtsFT897D::ReadByteFromPort(const uint16_t ATimeoutMS)
{
	uint32_t now = millis();
	while (!FPort.available()) {
		if (millis() - now > ATimeoutMS) return false;
	}
	return FPort.read();
}

void dtsFT897D::FlushPort(void) const
{
	FPort.flush();
}

dtsFT897D::dtsFT897D(SoftwareSerial& ASerialPort) : FPort(ASerialPort)
{
	ClearCmdBuffer();
}
