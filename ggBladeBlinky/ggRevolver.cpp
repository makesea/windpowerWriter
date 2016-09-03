#include "ggRevolver.h"


ggRevolver::ggRevolver(byte aSensorPin)
: mSensorPin(aSensorPin),
  mSensorLast(false),
  mMicros(0),
  mMicrosLast(0),
  mMicrosDelta(0)
{
}


bool ggRevolver::SensorRead()
{
  if (mSensorPin != A6 && mSensorPin != A7) return digitalRead(mSensorPin);
  else return analogRead(mSensorPin) >= 512;
}


void ggRevolver::Setup()
{
  pinMode(mSensorPin, INPUT_PULLUP);
}


void ggRevolver::Run()
{
  // measure the current time
  unsigned long vMicros = micros();
  mMicrosDelta = vMicros - mMicrosLast;
  // check if sensor is switching
  bool vSensor = SensorRead();  
  if (vSensor != mSensorLast) {
    // if sensor switched on, store new revolution time
    if (vSensor) {
      // do a bit of filtering
      mMicros = (2 * mMicros + mMicrosDelta) / 3;
      mMicrosLast = vMicros;
    }
    mSensorLast = vSensor;
  }
}


bool ggRevolver::IsOK() const
{
  return (mMicros > 0) &&
         (mMicrosDelta < 2 * mMicros);
}


unsigned long ggRevolver::GetMicros() const
{
  return mMicros;
}


unsigned long ggRevolver::GetMicrosDelta() const
{
  return mMicrosDelta;
}


float ggRevolver::GetAngleDeg() const
{
  return mMicros != 0 ? 360.0f * mMicrosDelta / mMicros : 0.0f;
}


float ggRevolver::GetAngleRad() const
{
  return mMicros != 0 ? 2.0f * M_PI * mMicrosDelta / mMicros : 0.0f;
}


int ggRevolver::GetRPM() const
{
  return  mMicros != 0 ? 60L*1000000L / mMicros : 0;
}


float ggRevolver::GetFrequency() const
{
  return mMicros != 0 ? 1000000.0f / mMicros : 0.0f;
}


float ggRevolver::GetSeconds() const
{
  return 0.001f * mMicros;
}

