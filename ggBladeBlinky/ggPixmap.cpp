#include "ggPixmap.h"
#include "ggFontBold.h"


ggPixmap::ggPixmap(int aSizeX, byte aTextScale, byte aCharSpace)
: mSizeX(aSizeX),
  mTextScale(aTextScale),
  mCharSpace(aCharSpace)
{
  mData = new byte[aSizeX];
  mFont = new ggFontBold();
  Clear();
}


ggPixmap::~ggPixmap()
{
  delete[] mData;
  delete mFont;
}


int ggPixmap::GetSizeX() const
{
  return mSizeX;
}


int ggPixmap::GetSizeY() const
{
  return 8;
}


void ggPixmap::SetTextScale(byte aScale)
{
  mTextScale = aScale;
}


void ggPixmap::SetCharSpace(byte aSpace)
{
  mCharSpace = aSpace;
}


void ggPixmap::Clear()
{
  memset(mData, 0, mSizeX);
}


void ggPixmap::SetPixel(int aPosX, int aPosY, bool aPixel)
{
  if (aPixel) mData[aPosX] |= 1 << aPosY;
  else mData[aPosX] &= ~(1 << aPosY);
}


bool ggPixmap::GetPixel(int aPosX, int aPosY) const
{
  return (mData[aPosX] >> aPosY) & 1;
}


void ggPixmap::SetColumn(int aPosX, byte aColumn)
{
  mData[aPosX] = aColumn;
}


byte ggPixmap::GetColumn(int aPosX) const
{
  return mData[aPosX];
}


void ggPixmap::SetText(const char* aText,
                       int aPosX,
                       bool aCenter,
                       bool aFlipX,                
                       bool aFlipY)
{
  // calculate text length (chars) and width (pixel)
  const char* vCharPtr = aText;
  int vTextLength = strlen(aText);
  int vTextWidth = (vTextLength - 1) * mCharSpace;
  for (int vCharIndex = 0; vCharIndex < vTextLength; vCharIndex++) {
    char vChar = *vCharPtr++;
    vTextWidth += mTextScale * mFont->GetCharWidth(vChar);
  }
  // render the text into pixmap
  int vTextWidth2 = 0;
  if (aCenter) aPosX -= vTextWidth / 2;
  vCharPtr = aText;
  for (int vCharIndex = 0; vCharIndex < vTextLength; vCharIndex++) {
    char vChar = *vCharPtr++;
    byte vCharWidth = mTextScale * mFont->GetCharWidth(vChar);
    const byte* vCharPixels = mFont->GetCharPixels(vChar);
    for (byte vColIndex = 0; vColIndex < vCharWidth; vColIndex++) {
      byte vPixmapColByte = 0;
      for (byte vRowIndex = 0; vRowIndex < 7; vRowIndex++) {
        vPixmapColByte <<= 1;
        if ((vCharPixels[aFlipY ? 6-vRowIndex : vRowIndex] >> (vColIndex/mTextScale)) & 1) vPixmapColByte |= 1;
      }
      int vIndex = aPosX;
      if (!aFlipX) vIndex += (vTextWidth2 + vCharWidth - vColIndex - mCharSpace);
      else vIndex += (vTextWidth - vTextWidth2 - vCharWidth + vColIndex);
      if (vIndex < 0) vIndex += mSizeX;
      vIndex %= mSizeX;
      mData[vIndex] = vPixmapColByte;
    }
    vTextWidth2 += vCharWidth + mCharSpace;
  }
}


void ggPixmap::SetText(const char* aTextA,
                       const char* aTextB)
{
  SetText(aTextA, 0, true, false, false);
  SetText(aTextB, mSizeX / 2, true, true, true);
}


void ggPixmap::SetSpeedRPM(int aRotationsPerMinute)
{
  char vBuffer[32];
  itoa(aRotationsPerMinute, vBuffer, 10);
  SetText(vBuffer, "RPM");
}


void ggPixmap::SetSpeedWind(float aSpeed)
{
  char vBuffer[32];
  dtostrf(aSpeed, 0, 1, vBuffer);
  strcat(vBuffer, " m/s");
  SetText("Wind:", vBuffer);
}


void ggPixmap::SetSpeedWindMPH(float aSpeed)
{
  char vBuffer[32];
  dtostrf(aSpeed, 0, 0, vBuffer);
  strcat(vBuffer, " MPH");
  SetText("Wind:", vBuffer);
}


void ggPixmap::SetStar(int aCorners)
{
  static const byte vBytes[8] = {
    B00000000,
    B00000001,
    B00000011,
    B00000111,
    B00001111,
    B00011111,
    B00111111,
    B01111111
  };
  for (long vIndex = 0; vIndex < mSizeX; vIndex++) {
    int vA = (2 * 8 * aCorners * vIndex) / mSizeX;
    int vB = vA % 16;
    if (vB > 7) vB = 15 - vB;
    mData[vIndex] = vBytes[vB];
  }
}


void ggPixmap::SetSpiral(int aLines,
                         int aWidth,
                         bool aFlip)
{
  for (int vLineIndex = 0; vLineIndex < aLines; vLineIndex++) {
    int vPosX = vLineIndex * mSizeX / aLines;
    byte vByte = aFlip ? B10000000 : B00000001;
    for (int vBit = 0; vBit < 8; vBit++) {
      for (int vX = 0; vX < aWidth; vX++) {
        mData[vPosX++] |= vByte;
        if (vPosX >= mSizeX) vPosX -= mSizeX;
      }
      vByte = aFlip ? (vByte >> 1) : (vByte << 1);
    }
  }
}


void ggPixmap::SetWatch()
{
  // get time (add some offset 10:12:27)
  const unsigned long vH = 10;
  const unsigned long vM = 12;
  const unsigned long vS = 27;
  unsigned long vSeconds = millis()/1000 + vS + vM*60 + vH*60*60;
  int vIndexH = (vSeconds * mSizeX / 60 / 60 / 12) % mSizeX;
  int vIndexM = (vSeconds * mSizeX / 60 / 60) % mSizeX;
  int vIndexS = (vSeconds * mSizeX / 60) % mSizeX;
  // day quarters
  for (int vIndex = 0; vIndex < 4; vIndex++) {
    int vPos = vIndex * mSizeX / 4 + mSizeX;
    mData[(vPos-1)%mSizeX] |= B01100000;
    mData[(vPos  )%mSizeX] |= B01110000;
    mData[(vPos+1)%mSizeX] |= B01100000;
  }
  // hours tockmarks
  for (int vIndex = 0; vIndex < 12; vIndex++) {
    int vPos = vIndex * mSizeX / 12;
    mData[vPos] |= B01100000;
  }
  // seconds tickmarks
  for (int vIndex = 0; vIndex < 60; vIndex++) {
    int vPos = vIndex * mSizeX / 60;
    mData[vPos] |= B01000000;
  }
  // hours hand
  mData[(vIndexH+mSizeX-4)%mSizeX] |= B00000001;
  mData[(vIndexH+mSizeX-3)%mSizeX] |= B00000011;
  mData[(vIndexH+mSizeX-2)%mSizeX] |= B00000111;
  mData[(vIndexH+mSizeX-1)%mSizeX] |= B00001111;
  mData[(vIndexH+mSizeX  )%mSizeX] |= B00001111;
  mData[(vIndexH+mSizeX+1)%mSizeX] |= B00001111;
  mData[(vIndexH+mSizeX+2)%mSizeX] |= B00000111;
  mData[(vIndexH+mSizeX+3)%mSizeX] |= B00000011;
  mData[(vIndexH+mSizeX+4)%mSizeX] |= B00000001;
  // minutes hand
  mData[(vIndexM+mSizeX-2)%mSizeX] |= B00011111;
  mData[(vIndexM+mSizeX-1)%mSizeX] |= B00111111;
  mData[(vIndexM+mSizeX  )%mSizeX] |= B00111111;
  mData[(vIndexM+mSizeX+1)%mSizeX] |= B00111111;
  mData[(vIndexM+mSizeX+2)%mSizeX] |= B00011111;
  // seconds hand
  mData[(vIndexS+mSizeX-1)%mSizeX] |= B00111111;
  mData[(vIndexS+mSizeX  )%mSizeX] |= B01111111;
  mData[(vIndexS+mSizeX+1)%mSizeX] |= B00111111;
}


#ifdef __DEBUG
void ggPixmap::SerialPrint()
{
  byte vBit = B01000000;
  for (byte vRow = 0; vRow < 7; vRow++) {
    for (int vIndex = 0; vIndex < mSizeX; vIndex++) {
      Serial.print(mData[vIndex] & vBit ? "X" : ".");
    }
    Serial.println();
    vBit >>= 1;
  }
}
#endif // __DEBUG


#ifdef __DEBUG
void ggPixmap::Test()
{
  Clear();
  SetStar(5);
  SerialPrint();
  Serial.println();
  
  Clear();
  SetTextScale(1); SetCharSpace(1);
  SetText("Goofy", 0, false, false, false);
  SerialPrint();
  Serial.println();
  
  Clear();
  SetTextScale(1); SetCharSpace(3);
  SetText("Goofy", 0, false, false, false);
  SerialPrint();
  Serial.println();
  
  Clear();
  SetTextScale(3); SetCharSpace(1);
  SetText("Goofy", 0, false, false, false);
  SerialPrint();
  Serial.println();

  Clear();
  SetTextScale(3); SetCharSpace(3);
  SetText("Goofy", 0, false, false, false);
  SerialPrint();
  Serial.println();
  
  Clear();
  SetTextScale(1); SetCharSpace(1);
  SetText("makeSEA", "rocks!");
  SerialPrint();
  Serial.println();
  
  Clear();
  SetSpeedRPM(1234);
  SerialPrint();
  Serial.println();
  
  Clear();
  SetSpeedWind(3.14f);
  SerialPrint();
  Serial.println();
  
  Clear();
  SetSpiral(7, GetSizeX() / 30);
  SerialPrint();
  Serial.println();
  
  Clear();
  SetWatch();
  SerialPrint();
  Serial.println();

  Clear();
}
#endif // __DEBUG

