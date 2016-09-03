#ifndef _GG_BUTTON_H__
#define _GG_BUTTON_H__

#include <Arduino.h>

class ggButton
{

public:
  
  ggButton(byte aPin,
           boolean aInverted = false,
           boolean aEnablePullUp = false,
           float aDecaySeconds = 0.025f);

  void begin();

  boolean IsOn() const;
  boolean IsOff() const;
  boolean SwitchingOn() const;
  boolean SwitchingOff() const;
  boolean Switching() const;
  
  boolean GetOn() const;
  boolean GetOff() const;
  long GetTimeOn() const;
  long GetTimeOff() const;
  long GetTime() const;
  
private:

  boolean ReadInput() const;

  const byte mPin;
  const boolean mInverted;
  const boolean mEnablePullUp;
  const float mDecaySeconds;
  
  mutable long mLastMicroSeconds;
  mutable float mInputIntegrated;
  mutable boolean mInput;
  mutable long mMilliSecondsLastChange;
  
};

#endif // _GG_BUTTON_H__
