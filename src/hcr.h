#pragma once

#ifndef HCRAPI_F
#define HCRAPI_F

#ifdef ARDUINO
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif
#endif

#include <String.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define HCR_BUFFER_SIZE 32

#ifndef HCR_BAUD_RATE
#define HCR_BAUD_RATE 9600
#endif

#ifndef HCR_I2C_RATE
#define HCR_I2C_RATE 400000
#endif

#define CONN_SERIAL0 0x00 //"Serial"
#define CONN_SERIAL1 0x01 //"Serial1"
#define CONN_SERIAL2 0x02 //"Serial2"
#define CONN_SERIAL3 0x03 //"Serial3"
#define CONN_SERIAL4 0x04 //"Serial4"
#define CONN_I2C     0x05 //"I2C"
#define CONN_I2C1    0x06 //"I2C1"
#define CONN_I2C2    0x07 //"I2C2"

#define HAPPY 0
#define SAD 1
#define MAD 2
#define SCARED 3
#define OVERLOAD 4

#define EMOTE_MODERATE 0
#define EMOTE_STRONG 1
#define FADE 1

#define CH_V 0
#define CH_A 1
#define CH_B 2

/*!
    @brief  Class that stores state and functions for interacting with
            Human Cyborg Relations Vocalizer and compatible devices.
*/
class HCRVocalizer
{
public:
    HCRVocalizer(const uint8_t addr, TwoWire &i2c);
    HCRVocalizer(const uint8_t addr, TwoWire &i2c, int baud);
    HCRVocalizer(HardwareSerial *conn,int baud);
    HCRVocalizer(SoftwareSerial *conn,int baud);
    HCRVocalizer(int rx, int tx, int baud);

    void begin(void);
    void begin(const uint16_t refspeed);
    void update(void);
    void Trigger(int e,int v);
    void Stimulate(int e,int v);
    void Overload(void);
    void Muse(void);
    void Muse(int min, int max);
    void Stop(void);
    void StopEmote(void);

    void OverrideEmotions(int v);
    void ResetEmotions(void);
    void SetEmotion(int e,int v);

    void SetMuse(int v);

    void PlayWAV(int v,int file);
    void PlayWAV(int v,String file);
    void StopWAV(int ch);
    void SetVolume(int e,int v);

    int* GetEmotions(void);
    int GetEmotion(int e);
    float GetDuration(void);
    int GetOverride(void);
    bool IsPlaying(void);
    bool IsPlaying(int ch);
    int GetMuse(void);
    int GetWAVCount(void);
    int GetPlayingWAV(int ch);
    float getVolume(int e);
    void getUpdate(void);

    void dfPlayer();

private:
    uint8_t _i2caddr=0;
    TwoWire *_i2c;
    HardwareSerial *_serial;
    SoftwareSerial *_softserial;
    int _serialBaud;
    int connectionType;

    void transmit(String command);
    void transmit(String command, bool retry);
    void receive(void);
    void sendCommand(String command);
    void receiveData(char ch);
    void receiveData(String data);
    void processCommands(char* input);
    String getValue(String data, char separator, int index);

protected:
    char hcrstartMarker = '<';
    char hcrendMarker = '>';
    char cmdBuffer[HCR_BUFFER_SIZE];
    unsigned cmdPos;
    byte connection;
    int refreshSpeed;
    int emote_happy;
    int emote_sad;
    int emote_mad;
    int emote_scared;
    int state_override;
    int state_musing;
    int state_files;
    float state_duration;
    int state_chv;
    int state_cha;
    int state_chb;
    int Volume_V;
    int Volume_A;
    int Volume_B;
};

using namespace std;
//ToString
inline String ToString (double d)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%g", d);
	return buffer;
}
inline String ToString (float d)
{
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%g", d);
	return buffer;
}
inline String ToString (char a)
{
	return String(a).c_str();
}
inline String ToString (const char* a)
{
	return String(a).c_str();
}
inline String ToString (int number)
{
	return String(number).c_str();
}
inline String ToString (int number, String thing)
{
	String Prefix = "";
	if (thing == "00")
	{
		if (number <= 9)
		{
			Prefix = "0";	
		}
	}
	return (Prefix + String(number)).c_str();
}
inline String ToString (bool d)
{
	if (d)
	{
		return "true";	
	}
	return "false";
}

#endif