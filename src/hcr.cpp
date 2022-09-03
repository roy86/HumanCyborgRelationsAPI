#include "hcr.h"

HCRVocalizer::HCRVocalizer(uint16_t n, byte connectiontype, int refreshRate)
: begun(false), connection(0x00), commandCache(""), refreshSpeed(0),
  emote_happy(0), emote_sad(0), emote_mad(0), emote_scared(0),
  state_override(0),state_musing(0),state_files(0),state_duration(0.00),
  state_chv(0),state_cha(0),state_chb(0)
{
    connection = connectiontype;
    refreshSpeed = refreshRate;
};

/*!
  @brief   Configure HCR API for output.
*/
void HCRVocalizer::begin(void) {
    switch (connection)
    {
        case 0x00: Serial.begin(HCR_BAUD_RATE); break;
        case 0x05:
            Wire.begin();
            Wire.setClock(400000);
            break;
    };
}

void HCRVocalizer::update(void) {
    begun = true;
    if (commandCache == "" && millis()%refreshSpeed == 0) {
        commandCache = "QD";
    }
    transmitCommand(commandCache);
    commandCache = "";
}

void HCRVocalizer::Stimulate(int e,int v) {
    if (e < 0 || e > 4) return;
    if (e == 4) {
        Overload();
    } else {
        char emoteprefix[] = "HSMC";
        string msg = "S" + ToString((char) emoteprefix[e]) + ToString(v);
        buildCommand(msg);
    };
}

void HCRVocalizer::Overload(void) {
    string msg = "SE";
    buildCommand(msg);
}

void HCRVocalizer::StopEmote(void) {
    string msg = "PSV";
    buildCommand(msg);
}

void HCRVocalizer::OverrideEmotions(int v) {
    if (v < 0 || v > 1) return;
    string msg = "O" + ToString(v);
    buildCommand(msg);
}

void HCRVocalizer::ResetEmotions(void) {
    string msg = "OR";
    buildCommand(msg);
}

void HCRVocalizer::SetEmotion(int e,int v) {
    if (e < 0 || e > 3) return;
    if (v < 0 || v > 99) return;
    char emoteprefix[] = "HSMC";
    string msg = "O" + ToString((char) emoteprefix[e]) + ToString(v);
    buildCommand(msg);
}

void HCRVocalizer::SetMuse(int v) {
    if (v < 0 || v > 1) return;
    string msg = "O" + ToString(v);
    buildCommand(msg);
}

void HCRVocalizer::PlayWAV(int ch,string file) {
    char channel[] = "VAB";
    string msg = "C" + ToString((char) channel[ch]) + file;
    buildCommand(msg);
}

void HCRVocalizer::StopWAV(int ch) {
    char channel[] = "VAB";
    string msg = "PS" + ToString((char) channel[ch]);
    buildCommand(msg);
}

void HCRVocalizer::SetVolume(int e,int v) {
    if (e < 0 || e > 2) return;
    char channel[] = "VAB";
    string msg = "PV" + ToString((char) channel[e]) + ToString(v);;
    buildCommand(msg);
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

/*! 
    //
*/
void HCRVocalizer::buildCommand(string cmd)
{
    if (commandCache.length() == 0) {
        commandCache += cmd;
    } else {
        commandCache += "," + cmd;
    };
}

void HCRVocalizer::transmitCommand(string cmd)
{
    if (cmd.length() == 0) return;

    if (cmd != "QD")
        cmd = cmd + ",QD";

    const char* hcr_inputc = ("<" + cmd + ">").c_str();
    bool query = false;

    //Serial.println(("<" + cmd + ">").c_str());

    switch (connection)
    {
        case 0x00:
            if (Serial.available()) {
                for (int i=0; i<9; i++) {
                    Serial.print((byte) hcr_inputc[i]);
                    if (hcr_inputc[i] == 'Q' && hcr_inputc[i+1] == 'D')
                        query = true;
                };
            };
            break;
        case 0x05:
            Wire.beginTransmission(0x10);
            for (int i=0; i<9; i++) {
                Wire.write((byte) hcr_inputc[i]);
                if (hcr_inputc[i] == 'Q' && hcr_inputc[i+1] == 'D')
                    query = true;
            }
            if (query) {
                getResponse();
            }
            Wire.endTransmission();
        break;
    };
}

String HCRVocalizer::getResponse(void) {
    String response = "";
    switch (connection)
    {
        case 0x00:
            if (Serial.available()) {
                //
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