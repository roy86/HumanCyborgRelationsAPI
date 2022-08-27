# Human Cyborg Relations C++ API
C++ based API for Michael Perl's Human Cyborg Relations Teensy Based Vocalizers

# Commands

### Setup Commands

#### HCRVocalizer
Created the HCR Vocalizer API for your device. In this demo we call it "HCR" but use what is best fit.

PARAM Variable | Port
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

### Interaction Commands

#### Stimulate()
Illicit response

Happy
```obj-c++
HCR.Stimulate(HAPPY,1);
```

Sad
```obj-c++
HCR.Stimulate(SAD,1);
```

Mad
```obj-c++
HCR.Stimulate(MAD,1);
```

Scared
```obj-c++
HCR.Stimulate(SCARED,1);
```

Overload (Electrocution)
```obj-c++
HCR.Stimulate(OVERLOAD,1);
```

### Overrides

#### OverrideEmotions()
Stops the normalisation of emotional states and allows each state to remain at its current level

Enable
```obj-c++
HCR.OverrideEmotions(1);
```

Disable
```obj-c++
HCR.OverrideEmotions(0);
```

#### ResetEmotions()
Resets all emotional states to 0

```obj-c++
HCR.ResetEmotions();
```

#### SetEmotion()
Sets the specific emotion score based on the integer of 0-99
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

#### GetEmotions()
Returns an integer array of the emote scores

```obj-c++
HCR.GetEmotions();
```

#### GetEmotion()
Returns int of the emote score for HAPPY out of 100

```obj-c++
HCR.GetEmotion(HAPPY);
```

Returns int of the emote score for SAD out of 100
```obj-c++
HCR.GetEmotion(SAD);
```

Returns int of the emote score for MAD out of 100
```obj-c++
HCR.GetEmotion(MAD);
```

Returns int of the emote score for SCARED out of 100
```obj-c++
HCR.GetEmotion(SCARED);
```

#### GetDuration()
Returns float of current emote duration

```obj-c++
HCR.GetDuration();
```

#### GetOverride()
Returns integer if override is enabled

```obj-c++
HCR.GetOverride();
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

#### SetMuse()
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

### SD CARD AUDIO

#### PlayWAV()
Play a WAV file from the SD from either A or B channel with either a name or the file number.

WAV Channel A with a specific file name
```obj-c++
HCR.GetMuse(CH_A,"String.WAV");
```

WAV Channel B with a file number
```obj-c++
HCR.GetMuse(CH_B,1);
```

Reminder all files must have a suffix of ..._<0000-9999>.WAV

#### StopWAV()
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

#### SetVolume()
Sets the float volume from 0-1 of each available channel

Vocalizer Channel
```obj-c++
HCR.SetVolume(CH_V,0.99);
```

WAV Channel A
```obj-c++
HCR.SetVolume(CH_A,0.5);
```

WAV Channel B
```obj-c++
HCR.SetVolume(CH_B,0.5);
```

