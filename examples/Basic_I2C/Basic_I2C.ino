// Include the HCR Library
#include <hcr.h>

// Initialise the HCR Vocalizer API
HCRVocalizer HCR(20,CONN_I2C,125);

void setup() {
  // Begin HCR Setup
  HCR.begin();

  // Enable Musing
  HCR.SetMuse(0);

  // Disable Emotion Override
  HCR.OverrideEmotions(0);
}

int Wait = 0;
void loop() {
  // If 10 Seconds have passed
  if (millis() > (Wait + 10000)) {

    // Stimulate Random Response
    HCR.Stimulate(random(0,4),random(0,2));

    // Reset Timer    
    Wait = millis();
  }

  HCR.update();

  int isplaying = HCR.IsPlaying();
  Serial.println(isplaying);
}