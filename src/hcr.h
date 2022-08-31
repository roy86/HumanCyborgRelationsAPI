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

#include <iostream>
#include <Wire.h>

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
    HCRVocalizer(uint16_t n, byte connectiontype, int refreshSpeed);

    void begin(void);
    void update(void);
    void Stimulate(int e,int v);
    void Overload(void);
    void StopEmote(void);

    void OverrideEmotions(int v);
    void ResetEmotions(void);
    void SetEmotion(int e,int v);

    void SetMuse(int v);

    void PlayWAV(void);
    void StopWAV(void);
    void SetVolume(int e,int v);

    int* GetEmotions(void);
    int  GetEmotion(int e);
    float GetDuration(void);
    int GetOverride(void);
    int IsPlaying(void);
    int GetMuse(void);
    int GetWAVCount(void);
    int GetPlayingWAV(int ch);

private:
    void buildCommand(std::string cmd);
    void transmitCommand(std::string cmd);
    String getResponse(void);
    void updatedata(std::string df);
    String getValue(String data, char separator, int index);

protected:
    uint16_t deviceID;
    bool begun;
    byte connection;
    std::string commandCache;
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

};

using namespace std;
//ToString
inline string ToString (double d)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%g", d);
	return buffer;
}
inline string ToString (float d)
{
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%g", d);
	return buffer;
}
inline string ToString (char a)
{
	return String(a).c_str();
}
inline string ToString (const char* a)
{
	return String(a).c_str();
}
inline string ToString (int number)
{
	return String(number).c_str();
}
inline string ToString (int number, string thing)
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
inline string ToString (bool d)
{
	if (d)
	{
		return "true";	
	}
	return "false";
}

#endif