// Include the HCR Library
#include <hcr.h>

// Initialise the HCR Vocalizer API
HCRVocalizer HCR(20,CONN_SERIAL0,250);

void setup() {
  // put your setup code here, to run once:

  // Begin HCR Setup
  HCR.begin();

  // Enable Musing
  HCR.SetMuse(0);

  // Disable Emotion Override
  HCR.OverrideEmotions(0);
}

int Wait = 0;
void loop() {
  // put your main code here, to run repeatedly:

  // If 10 Seconds have passed
  if (millis() > (Wait + 10000)) {

    // Stimulate a happy response
    HCR.Stimulate(HAPPY,0);

    // Stimulate Random Response
    //HCR.Stimulate(random(0,4),random(0,2));

    // Reset Timer    
    Wait = millis();
  }

  HCR.update();

  int isplaying = HCR.IsPlaying();
  Serial.println(isplaying);
}