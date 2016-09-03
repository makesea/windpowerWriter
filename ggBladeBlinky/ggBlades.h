#ifndef __GG_BLADES_H__
#define __GG_BLADES_H__

#include <Arduino.h>

class ggBlades {

public:

  ggBlades(byte aNumBlades, byte aNumLEDs = 8);
  virtual ~ggBlades();

  byte GetNumBlades() const;
  byte GetNumLEDs() const;
  
  void SetPins(byte aBladeIndex, const byte* aPins);
  void Setup();
  
  void Write(byte aBladeIndex, byte aByte);
  
private:

  const byte mNumBlades;
  const byte mNumLEDs;
  const byte** mPins;
  
};

#endif // __GG_BLADES_H__
