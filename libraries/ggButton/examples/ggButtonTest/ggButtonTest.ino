#include "ggButton.h"

#define LED_PIN 13

ggButton mButton(8, true, true);

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  mButton.begin();
}

void loop()
{
  if (mButton.SwitchingOn()) {
    static boolean vLedOn = true;
    digitalWrite(LED_PIN, vLedOn);
    vLedOn = !vLedOn;
  }
}

