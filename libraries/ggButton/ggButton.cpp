#include "ggButton.h"


ggButton::ggButton(byte aPin,
                   boolean aInverted,
                   boolean aEnablePullUp,
                   float aDecaySeconds)
: mPin(aPin),
  mInverted(aInverted),
  mEnablePullUp(aEnablePullUp),
  mDecaySeconds(aDecaySeconds),
  mLastMicroSeconds(0),
  mInputIntegrated(0.0f),
  mMilliSecondsLastChange(0),
  mInput(false)
{
}


void ggButton::begin()
{
  pinMode(mPin, INPUT);
  digitalWrite(mPin, mEnablePullUp);
  mLastMicroSeconds = micros();
  mInput = digitalRead(mPin);
  mInputIntegrated = mInput;
  mMilliSecondsLastChange = millis();
}


boolean ggButton::ReadInput() const
{
  long vMicroSeconds = micros();
  float vDeltaSeconds = 0.000001f * (vMicroSeconds - mLastMicroSeconds);
  mLastMicroSeconds = vMicroSeconds;
  float vDecay = mDecaySeconds / (mDecaySeconds + vDeltaSeconds);
  mInputIntegrated = vDecay * mInputIntegrated + (1.0f - vDecay) * digitalRead(mPin);
  boolean vInput = mInputIntegrated > (mInput ? 0.25f : 0.75f);
  if (vInput != mInput) {
    mMilliSecondsLastChange = millis();
    mInput = vInput;
  }
  return mInput;
}


boolean ggButton::IsOn() const
{
  return mInverted ^ ReadInput();
}

  
boolean ggButton::IsOff() const
{
  return mInverted ^ !ReadInput();
}


boolean ggButton::SwitchingOn() const
{
  return Switching() && GetOn();
}


boolean ggButton::SwitchingOff() const
{
  return Switching() && GetOff();
}


boolean ggButton::Switching() const
{
  boolean vInput = mInput;
  return vInput != ReadInput();
}


boolean ggButton::GetOn() const
{
  return mInverted ^ mInput;
}


boolean ggButton::GetOff() const
{
  return mInverted ^ !mInput;
}


long ggButton::GetTimeOn() const
{
  return GetOn() ? GetTime() : -1;
}


long ggButton::GetTimeOff() const
{
  return GetOff() ? GetTime() : -1;
}


long ggButton::GetTime() const
{
  return millis() - mMilliSecondsLastChange;
}
