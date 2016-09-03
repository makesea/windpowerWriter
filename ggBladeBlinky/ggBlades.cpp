#include "ggBlades.h"


ggBlades::ggBlades(byte aNumBlades, byte aNumLEDs)
: mNumBlades(aNumBlades),
  mNumLEDs(aNumLEDs)
{
  mPins = new const byte*[mNumBlades];
}


ggBlades::~ggBlades()
{
  delete[] mPins;
}


byte ggBlades::GetNumBlades() const
{
  return mNumBlades;
}


byte ggBlades::GetNumLEDs() const
{
  return mNumLEDs;
}


void ggBlades::SetPins(byte aBladeIndex, const byte* aPins)
{
  mPins[aBladeIndex] = aPins;
}


void ggBlades::Setup()
{
  for (byte vBladeIndex = 0; vBladeIndex < mNumBlades; vBladeIndex++) {
    for (byte vLEDIndex = 0; vLEDIndex < mNumLEDs; vLEDIndex++) {
      pinMode(mPins[vBladeIndex][vLEDIndex], OUTPUT);
      digitalWrite(mPins[vBladeIndex][vLEDIndex], LOW);
    }
  }
}


void ggBlades::Write(byte aBladeIndex, byte aByte)
{
  byte vMask = 1;
  for (byte vLEDIndex = 0; vLEDIndex < mNumLEDs; vLEDIndex++) {
    digitalWrite(mPins[aBladeIndex][vLEDIndex], (aByte & vMask) != 0);
    vMask <<= 1;
  }
}

