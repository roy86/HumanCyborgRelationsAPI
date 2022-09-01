# Human Cyborg Relations C++ API
C++ based API for interfacing with Michael Perl's [Human Cyborg Relations](https://humancyborgrelations.com/r2d2/) Teensy Based Vocalizers

## Table of contents
* [Overview](#Overview)
* [Hardware](#hardware)
* [Commands](#Commands)
* [> Setup](#Setup)
* [> Emote](#emotion-commands)
* [> Override](#overrides)
* [> Musing](#musing)
* [> SDCARD](#sdcard-audio)
* [> Volume](#volume-controls)

## Overview

[Human Cyborg Relations](https://humancyborgrelations.com/r2d2/) created by Michael Perl, has changed the face of Droid Vocalization through a fully functional AI, powered by advanced research into the speech patterns and emotional logic of R2-D2.

Each vocalization is unique. A sophisticated algorithm assembles R2’s speech "phoneme" by "phoneme." Billions upon billions of rules dictate how each sound should be selected, processed, timed, and sequenced to achieve emotional and canonical accuracy.

The [Human Cyborg Relations R2 Vocalizer](https://humancyborgrelations.com/r2d2/) has been ported to the Teensy 4.1 Hardware for stand-alone integration into custom droids build for the Astromech community usage.

All proceeds support [Force for Change](https://www.starwars.com/force-for-change) and [FIRST®](https://www.firstinspires.org/)

## Hardware

---

## Commands

### Setup

#### HCRVocalizer
Created the HCR Vocalizer API for your device. In this demo we call it "HCR" but use what is best fit.

PARAM Variable | Port
--- | ---
CONN_SERIAL0 | Serial
CONN_SERIAL1 | Serial1
CONN_SERIAL2 | Serial2
CONN_SERIAL3 | Serial3
CONN_SERIAL4 | Serial4
CONN_I2C | I2C

Serial 4 Connection with HCR as Serial Bus ID#: 40
```obj-c++
HCRVocalizer HCR(40,CONN_SERIAL4);
```

I2C Connection with HCR as bus ID: 0x02
```obj-c++
HCRVocalizer HCR(0x02,CONN_I2C);
```

#### Begin()
Used in the setup function to create the needed connections
```obj-c++
class Setup() {
    HCR.begin();
}
```
---
### Emotion Commands
These commands can be run in setup or in a loop function. Commands used in the `setup()` are not sent to the HCR Board until the loop is first run.

#### Stimulate(<int/string>,<int>)
Stimulate a response from the HCR AI with either a moderate or strong response.

Emotion Categories to use
Emotion | Value | String
--- | --- | ---
HAPPY | 0 | "Happy"
SAD | 1 | "Sad"
MAD | 2 | "Mad"
SCARED | 3 | "Scared"

Emotion Strength Setting
Variable | Value
--- | ---
EMOTE_MODERATE | 0
EMOTE_STRONG | 1

Happy
```obj-c++
HCR.Stimulate(HAPPY,EMOTE_MODERATE);
// or
HCR.Stimulate(HAPPY,0);
```

Sad
```obj-c++
HCR.Stimulate(SAD,EMOTE_STRONG);
```

Mad
```obj-c++
HCR.Stimulate(MAD,EMOTE_MODERATE);
```

Scared
```obj-c++
HCR.Stimulate(SCARED,EMOTE_MODERATE);
```

Overload (Electrocution)
```obj-c++
HCR.Stimulate(OVERLOAD);
```

#### GetEmotions()
Returns an integer array of all the emote scores

Happy | Sad | Mad | Scared
--- | --- | --- | ---
`INT` 0-99 | `INT` 0-99 | `INT` 0-99 | `INT` 0-99

```obj-c++
HCR.GetEmotions();
```

#### GetEmotion(<int>)
Returns int of the emote score for HAPPY out of `100`

```obj-c++
HCR.GetEmotion(HAPPY);
```

Returns int of the emote score for SAD out of `100`
```obj-c++
HCR.GetEmotion(SAD);
```

Returns int of the emote score for MAD out of `100`
```obj-c++
HCR.GetEmotion(MAD);
```

Returns int of the emote score for SCARED out of `100`
```obj-c++
HCR.GetEmotion(SCARED);
```

#### GetDuration()
Returns float of current emote duration

```obj-c++
HCR.GetDuration();
```

#### StopEmote()
Stops the current Emote from playing

```obj-c++
HCR.StopEmote();
```

#### IsPlaying()
Returns a binary integer if a vocalisation is playing
```obj-c++
HCR.IsPlaying();
```

---

### Overrides

#### OverrideEmotions(<int>)
Stops the normalisation of emotional states and allows each state to remain at its current level

Enable
```obj-c++
HCR.OverrideEmotions(1);
```

Disable
```obj-c++
HCR.OverrideEmotions(0);
```

#### GetOverride()
Returns integer if override is enabled

```obj-c++
HCR.GetOverride();
```

#### SetEmotion(<int>,<int>)
Sets the specific emotion score based on the integer of `0-99`
Reminder that without override, the scores will normalise to 0 over time

```obj-c++
HCR.SetEmotion(HAPPY,89);
```

```obj-c++
HCR.SetEmotion(SAD,22);
```

```obj-c++
HCR.SetEmotion(MAD,0);
```

```obj-c++
HCR.SetEmotion(SCARED,0);
```

#### ResetEmotions()
Resets all emotional states to `0`

```obj-c++
HCR.ResetEmotions();
```

---

### Musing

#### SetMuse(<int>)
Turn on/off random periodical mumbling audio animation

Enable
```obj-c++
HCR.SetMuse(1);
```

Disable
```obj-c++
HCR.SetMuse(0);
```

#### GetMuse()
Returns the binary integer state if Muse is enabled or disabled

```obj-c++
HCR.GetMuse();
```

---
### SDCARD AUDIO

`Reminder all files must have a suffix of ..._<0000-9999>.WAV`

#### PlayWAV(<Channel>,<File>)
Play a WAV file from the SD from either A or B channel with either a name or the file number.

Channel | Value | String | Description
--- | --- | --- | ---
CH_V | 0 | "Vocalizer" | Vocalizer Channel
CH_A | 0 | "WAV_A" | WAV Channel A
CH_B | 0 | "WAV_B" | WAV Channel B


WAV Channel A with a specific file name
```obj-c++
HCR.PlayWAV(CH_A,"String.WAV");
```

WAV Channel B with a file number
```obj-c++
HCR.PlayWAV(CH_B,1);
```

#### StopWAV(<int/string>,<int>)
Stops the WAV file being played and if it should be slowly faded out

WAV Channel A
```obj-c++
HCR.StopWAV(CH_A,FADE);
```

WAV Channel B
```obj-c++
HCR.StopWAV(CH_B);
```

#### GetWAVCount()
Returns an interger of the number of compatible WAV files detected on the SD Card

```obj-c++
HCR.GetWAVCount();
```

#### GetPlayingWAV()
Returns the ID number of the WAV file playing or returns -1 if nothing is playing

WAV Channel A
```obj-c++
HCR.GetPlayingWAV(CH_A);
```

WAV Channel B
```obj-c++
HCR.GetPlayingWAV(CH_B);
```

---
### Volume Controls

#### SetVolume(<int/string>,<int>)
`TBD` Sets the float volume from 0-1 of each available channel

Vocalizer Channel
```obj-c++
HCR.SetVolume(CH_V,9);
```

WAV Channel A
```obj-c++
HCR.SetVolume(CH_A,5);
```

WAV Channel B
```obj-c++
HCR.SetVolume(CH_B,5);
```

