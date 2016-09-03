#ifndef __GG_FONT__
#define __GG_FONT__

#include <Arduino.h>

class ggFont
{
  
public:
  
  virtual byte GetFontHeight() const = 0;
  virtual byte GetCharWidth(char aChar) const = 0;
  virtual const byte* GetCharPixels(char aChar) const = 0;

};

#endif // __GG_FONT__
