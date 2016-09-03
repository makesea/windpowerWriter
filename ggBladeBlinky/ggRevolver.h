#ifndef __GG_REVOLVER_H__
#define __GG_REVOLVER_H__

#include <Arduino.h>

class ggRevolver {

public:

  ggRevolver(byte aSensorPin);

  void Setup();
  void Run();
  bool IsOK() const;
  
  unsigned long GetMicros() const;
  unsigned long GetMicrosDelta() const;

  float GetAngleDeg() const;
  float GetAngleRad() const;
  
  int GetRPM() const;
  float GetFrequency() const;
  float GetSeconds() const;

private:

  bool SensorRead();

  const byte mSensorPin;
  bool mSensorLast;
  unsigned long mMicros;
  unsigned long mMicrosLast;
  unsigned long mMicrosDelta;

};

#endif // __GG_REVOLVER_H__
