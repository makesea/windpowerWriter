#ifndef __GG_PIXMAP_H__
#define __GG_PIXMAP_H__

#include <Arduino.h>

//#define __DEBUG

class ggFont;

/**
 * binary bitmap with 8 pixel height and dynamic width
 */
class ggPixmap {

public:

  ggPixmap(int aSizeX, byte aTextScale = 1, byte aCharSpace = 1);
  virtual ~ggPixmap(); 

  int GetSizeX() const;
  int GetSizeY() const;

  void Clear();

  void SetPixel(int aPosX, int aPosY, bool aPixel);
  bool GetPixel(int aPosX, int aPosY) const;

  void SetColumn(int aPosX, byte aByte);
  byte GetColumn(int aPosX) const;

  void SetTextScale(byte aScale);
  void SetCharSpace(byte aSpace);

  void SetText(const char* aText,
               int aPosX = 0,
               bool aCenter = false,
               bool aFlipX = false,                
               bool aFlipY = false);

  void SetText(const char* aTextA,
               const char* aTextB);

  void SetSpeedRPM(int aRotationsPerMinute);

  void SetSpeedWind(float aSpeed);
  void SetSpeedWindMPH(float aSpeedMPH);

  void SetStar(int aCorners);
  
  void SetSpiral(int aLines = 5,
                 int aWidth = 5,
                 bool aFlip = true);

  void SetWatch();

#ifdef __DEBUG
  void SerialPrint();
  void Test();
#endif // __DEBUG

private:

  const int mSizeX;
  byte* mData;

  ggFont* mFont;
  byte mTextScale;
  byte mCharSpace;
};

#endif // __GG_PIXMAP_H__
