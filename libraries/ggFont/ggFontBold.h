#ifndef __GG_FONT_BOLD__
#define __GG_FONT_BOLD__

#include <Arduino.h>
#include "ggFont.h"

class ggFontBold : public ggFont
{
  
public:
  
  virtual byte GetFontHeight() const;
  virtual byte GetCharWidth(char aChar) const;
  virtual const byte* GetCharPixels(char aChar) const;
  
private:
  
  byte GetCharIndex(char aChar) const;
  
  static const byte mCharSize = 140 - 32 + 1;
  static const byte mChars[mCharSize][9];
  
};

#endif // __GG_FONT_BOLD__
