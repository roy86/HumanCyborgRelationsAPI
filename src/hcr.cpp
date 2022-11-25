#include "hcr.h"
#include <Stream.h>
#include <Wire.h>

#if defined(ARDUINO) && ARDUINO >=100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <avr/pgmspace.h>
#define F(str) (str)
#endif

#ifdef ARDUINO_ARCH_AVR
#include <SoftwareSerial.h>
#endif

#ifdef ARDUINO_ARCH_PIC32
#include <SoftwareSerial.h>
#endif

/*!
  @brief   Start the HCR API
*/
/* HCRVocalizer::HCRVocalizer(uint16_t n, byte connectiontype, int refreshRate)
: begun(false), connection(0x00), commandCache(""), refreshSpeed(0),
  emote_happy(0), emote_sad(0), emote_mad(0), emote_scared(0),
  state_override(0),state_musing(0),state_files(0),state_duration(0.00),
  state_chv(0),state_cha(0),state_chb(0), deviceID(n)
{
    connection = connectiontype;
    refreshSpeed = refreshRate;
}; */

HCRVocalizer::HCRVocalizer(const uint8_t addr, TwoWire &i2c)
    : _i2caddr(addr), _i2c(&i2c), _serialBaud(400000)
{
    connectionType=0x03;
}

HCRVocalizer::HCRVocalizer(const uint8_t addr, TwoWire &i2c, int baud)
    : _i2caddr(addr), _i2c(&i2c), _serialBaud(baud)
{
    connectionType=0x03;
}

HCRVocalizer::HCRVocalizer(HardwareSerial *conn,int baud)
    : _serial(conn), _serialBaud(baud)
{
    usehardwareserial = true;
    connectionType=0x01;
}

HCRVocalizer::HCRVocalizer(SoftwareSerial *conn,int baud)
    : _softserial(conn), _serialBaud(baud)
{
    usehardwareserial = false;
    connectionType=0x02;
}

HCRVocalizer::HCRVocalizer(int rx, int tx,int baud)
    : _serialBaud(9600)
{
    #if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_PIC32)
    usehardwareserial = false;
    connectionType=0x02;
    _softserial = new SoftwareSerial(rx,tx);
    #endif
}

/*!
  @brief    Begin the Human-Cyborg Relations Controller API
*/
void HCRVocalizer::begin(void)
{
    begin(125); // Default 125ms Refresh
}

/*!
  @brief    Begin the Human-Cyborg Relations Controller API
*/
void HCRVocalizer::begin(const uint16_t refspeed)
{
    switch (connectionType)
    {
        case 0x01:
#ifdef ARDUINO_ARCH_AVR
            _serial->begin(_serialBaud);
#elif defined ARDUINO_ARCH_SAM
            ((USARTClass *)_serial)->begin(_serialBaud);
#elif defined ARDUINO_ARCH_SAMD  // Arduino Zero, Zero Pro, M0 and M0 Pro
            ((HardwareSerial *)_serial)->begin(_serialBaud);
#elif defined ARDUINO_ARCH_PIC32
            _serial->begin(_serialBaud);
#else
            _serial->begin(_serialBaud);
#endif
            while (!_serial);
            _serial->println("Begin _serial");
            break;
        case 0x02:
#ifdef ARDUINO_ARCH_AVR
            ((SoftwareSerial *)_softserial)->begin(_serialBaud);
#elif defined ARDUINO_ARCH_PIC32
            ((SoftwareSerial *)_softserial)->begin(_serialBaud);
#endif
            while (!_softserial);
            _softserial->println("Begin _softserial");
            break;
        case 0x03:
            if (_i2caddr>0){
                _i2c->begin();
                _i2c->setClock(_serialBaud);
                _i2c->setWireTimeout(3000 /* us */, true /* reset_on_timeout */);
            }
            break;
        default:
            Serial.print("HCR: No Connection Setup Specified");
            break;
    }
}

void HCRVocalizer::update(void)
{
    transmit((String)"",false);
    //receive();
}

void HCRVocalizer::sendCommand(String command)
{
    command = hcrstartMarker+command+hcrendMarker;
    transmit(command,false);
}

void HCRVocalizer::transmit(String command)
{
    transmit(command,false);
}

void HCRVocalizer::transmit(String command, bool retry)
{
    if (command == "")
    {
        return;
    }

    int i2cStatus = 0;
    switch (connectionType)
    {
        case 0x01: _serial->println((command + "\n").c_str()); break;
        case 0x02: _softserial->println((command + "\n").c_str()); break;
        case 0x03:
            _i2c->beginTransmission((byte)_i2caddr);
            _i2c->write((command + "\n").c_str());
            i2cStatus = _i2c->endTransmission();
            Serial.print(command); Serial.print("-"); Serial.print(i2cStatus); Serial.println(";");
            if (i2cStatus > 2)
            {
                _i2c->endTransmission();
                if (!retry)
                {
                    delay(100);
                    Serial.print("->");
                    transmit(command,true);
                }
            }
            break;
        default: break;
    }
}

void HCRVocalizer::receive(void)
{
    _serial->flush();
    while (_serial->available())
    {
        char ch = _serial->read();
    }

    if (_i2caddr>0){
    }
}

//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------

void HCRVocalizer::Trigger(int e, int v)
{
    Stimulate(e,v);
}

void HCRVocalizer::Stimulate(int e, int v)
{
    if (e < 0 || e > 4) return;
    if (e == 4) {
        Overload();
    } else {
        char emoteprefix[] = "HSMC";
        String msg = "S" + ToString((char) emoteprefix[e]) + ToString(v);
        sendCommand(msg);
    }
}

void HCRVocalizer::Overload(void)
{
    String msg = "SE";
    sendCommand(msg);
}

void HCRVocalizer::Stop(void)
{
    StopEmote();
    StopWAV(0);
    StopWAV(1);
    StopWAV(2);
}

void HCRVocalizer::StopEmote(void) {
    String msg = "PSV";
    sendCommand(msg);
}

void HCRVocalizer::OverrideEmotions(int v) {
    if (v < 0 || v > 1) return;
    String msg = "O" + ToString(v);
    sendCommand(msg);
}

void HCRVocalizer::ResetEmotions(void) {
    String msg = "OR";
    sendCommand(msg);
}

void HCRVocalizer::SetEmotion(int e,int v) {
    if (e < 0 || e > 3) return;
    if (v < 0 || v > 99) return;
    char emoteprefix[] = "HSMC";
    String msg = "O" + ToString((char) emoteprefix[e]) + ToString(v);
    sendCommand(msg);
}

void HCRVocalizer::SetMuse(int v) {
    if (v < 0 || v > 1) return;
    String msg = "O" + ToString(v);
    sendCommand(msg);
}

void HCRVocalizer::PlayWAV(int ch,String file) {
    char channel[] = "VAB";
    String msg = "C" + ToString((char) channel[ch]) + file;
    sendCommand(msg);
}

void HCRVocalizer::StopWAV(int ch) {
    char channel[] = "VAB";
    String msg = "PS" + ToString((char) channel[ch]);
    sendCommand(msg);
}

void HCRVocalizer::SetVolume(int e,int v) {
    if (e < 0 || e > 2) return;
    char channel[] = "VAB";
    String msg = "PV" + ToString((char) channel[e]) + ToString(v);
    sendCommand(msg);
}

/*!
    Query
*/
int* HCRVocalizer::GetEmotions(void) {
    static int response[4] = {
        (int)emote_happy,
        (int)emote_sad,
        (int)emote_mad,
        (int)emote_scared
    };
    return response;
}

int HCRVocalizer::GetEmotion(int e) {
    if (e < 0 || e > 3) return 0;
    int response[4] = {
        (int)emote_happy,
        (int)emote_sad,
        (int)emote_mad,
        (int)emote_scared
    };
    return response[e];
}

float HCRVocalizer::GetDuration(void) {
    float duration = state_duration;
    return duration;
}

int HCRVocalizer::GetOverride(void) {
    int override = state_override;
    return override;
}

int HCRVocalizer::IsPlaying(void) {
    int playing = state_chv;
    return playing;
}

int HCRVocalizer::GetMuse(void) {
    int musing = state_musing;
    return musing;
}

int HCRVocalizer::GetWAVCount(void) {
    int wavcount = state_files;
    return wavcount;
}

int HCRVocalizer::GetPlayingWAV(int ch) {
    float wavplaying = 0;
    if (ch == 1) {
        wavplaying = state_cha;
    } else if (ch == 2) {
        wavplaying = state_chb;
    } else {
        wavplaying = state_chv;
    };
    return wavplaying;
}

String HCRVocalizer::getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/*! 
    //
*/

String HCRVocalizer::getResponse(void) {
    String response = "";
    switch (connection)
    {
        case 0x00:
            if (Serial.available())
            {
                char character;
                bool startcommand = false;
                bool receivingcommand = false;

                while (Serial.available())
                {
                    static byte ndx = 0;
                    character = Serial.read();

                    if (character == hcrendMarker || character == '\0')
                        startcommand = false;

                    if (startcommand || receivingcommand)
                    {                
                        receivingcommand = startcommand;
                        if (receivingcommand)
                        {
                            response.concat(character);
                        };
                    }

                    if (character == hcrstartMarker)
                        startcommand = true;
                }
                //Serial.println(response);
            };
            break;
        case 0x05:
            Wire.requestFrom(0x10,48);
            if (Wire.available()) {
                char character;
                int wirelen = int(Wire.read());

                /* Serial.print("L(");
                Serial.print(wirelen);
                Serial.print("):"); */

                while (Wire.available() && wirelen+2 > 0)
                {
                    character = Wire.read();
                    response.concat(character);
                    wirelen--;
                }

                if (Wire.available())
                {
                    while (Wire.available()) {Wire.read();};
                };
                //Serial.println(response);
            };
            break;
        default: break;
    }

    if (response != "") {
        emote_happy = getValue(response,',',1).toInt();
        emote_sad = getValue(response,',',2).toInt();
        emote_mad = getValue(response,',',3).toInt();
        emote_scared = getValue(response,',',4).toInt();
        state_override = getValue(response,',',5).toInt();
        state_musing = getValue(response,',',6).toInt();
        state_files = getValue(response,',',7).toInt();
        state_duration = getValue(response,',',8).toFloat();
        state_chv = getValue(response,',',9).toInt();
        state_cha = getValue(response,',',10).toInt();
        state_chb = getValue(response,',',11).toInt();
    }

    return response;
}