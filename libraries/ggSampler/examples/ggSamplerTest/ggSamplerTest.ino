#include "ggSampler.h"

#define LED_A_PIN A3
#define LED_B_PIN A4

ggSampler mBlinkerA(1.0f);
ggSampler mBlinkerB(3.0f);

boolean mLightA = false;
boolean mLightB = false;

void setup()
{
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
}

void loop()
{
  if (mBlinkerA.IsDue()) {
    digitalWrite(LED_A_PIN, mLightA);
    mLightA = !mLightA;
  }
  if (mBlinkerB.IsDue()) {
    digitalWrite(LED_B_PIN, mLightB);
    mLightB = !mLightB;
  }
}
