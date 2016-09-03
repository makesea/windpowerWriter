//#define __DEBUG

#include "ggBlades.h"
#include "ggPixmap.h"
#include "ggRevolver.h"
#include "ggSampler.h"

ggPixmap mPixmap(120, // 120 pixel width (perimeter)
                 1,   // 1:1 text width scale
                 1);  // 1 pixel space between two chars

#define NumBlades 3
#define NumLEDs 7

// controls LEDs on propeller blades
ggBlades mBlades(NumBlades, NumLEDs);

const byte mPinsBladeA[NumLEDs] = { 8,  9, 10, 11, 12, 13, A0};
const byte mPinsBladeB[NumLEDs] = { 1,  2,  3,  4,  5,  6,  7};
const byte mPinsBladeC[NumLEDs] = {A1, A2, A3, A4, A5,  0, A7};

// reads position sensor and computes revolution time
ggRevolver mRevolver(A6);

// last rendered position (for propeller blades)
int mLastPosX = 0;

// pixel offset correpsonding to angle between magnet and first blade
const int mOffsetX = 8.0f / 360.0f * mPixmap.GetSizeX();

// propeller pitch for wind speed calculation (meter per revolution)
const float mPropellerPitch = 0.5f; 

// generates events at a specified frequency, used to change screens
ggSampler mSampler(1.0f);


/** 
 * copy pixmap to LED in blades (depemding on current rotation angle 
 */
void RenderBlades()
{
  long vPosX = mOffsetX + mPixmap.GetSizeX() * mRevolver.GetMicrosDelta() / mRevolver.GetMicros();
  if (vPosX != mLastPosX) {
    const int vBladeOffsetX = mPixmap.GetSizeX() / mBlades.GetNumBlades();
    for (int vBladeIndex = 0; vBladeIndex < mBlades.GetNumBlades(); vBladeIndex++) {
      int vX = vPosX + vBladeIndex*vBladeOffsetX;
      while (vX < 0) vX += mPixmap.GetSizeX();
      while (vX >= mPixmap.GetSizeX()) vX -= mPixmap.GetSizeX();
      mBlades.Write(vBladeIndex, mPixmap.GetColumn(vX));
    }
    mLastPosX = vPosX;
  }
}


/**
 * switch from screen to screen (like a slide-show), show various nice images and measurements
 */ 
void BrowseScreens()
{
  static int vScreenIndex = 0;
  switch (vScreenIndex) {
      
    case 0: mPixmap.Clear(); mPixmap.SetText("makeSEA", "rocks!"); break;
    case 1: mPixmap.Clear(); mPixmap.SetText("makeSEA", "rocks!"); break;
    case 2: mPixmap.Clear(); mPixmap.SetText("makeSEA", "rocks!"); break;

    case 3: mPixmap.Clear(); mPixmap.SetStar(5); break;

    case 4: mPixmap.Clear(); mPixmap.SetText("R.O.M.E.", "rocks!"); break;
    case 5: mPixmap.Clear(); mPixmap.SetText("R.O.M.E.", "rocks!"); break;
    case 6: mPixmap.Clear(); mPixmap.SetText("R.O.M.E.", "rocks!"); break;

    case 7: mPixmap.Clear(); mPixmap.SetSpiral(7, mPixmap.GetSizeX()/35, true); break;

    case 8: mPixmap.Clear(); mPixmap.SetSpeedRPM(mRevolver.GetRPM()); break;
    case 9: mPixmap.Clear(); mPixmap.SetSpeedRPM(mRevolver.GetRPM()); break;
    case 10: mPixmap.Clear(); mPixmap.SetSpeedRPM(mRevolver.GetRPM()); break;

    case 11: mPixmap.Clear(); mPixmap.SetStar(4); break;

    case 12: mPixmap.Clear(); mPixmap.SetSpeedWind(mPropellerPitch * mRevolver.GetFrequency()); break;
    case 13: mPixmap.Clear(); mPixmap.SetSpeedWind(mPropellerPitch * mRevolver.GetFrequency()); break;
    case 14: mPixmap.Clear(); mPixmap.SetSpeedWind(mPropellerPitch * mRevolver.GetFrequency()); break;
    case 15: mPixmap.Clear(); mPixmap.SetSpeedWindMPH(2.237f * mPropellerPitch * mRevolver.GetFrequency()); break;
    case 16: mPixmap.Clear(); mPixmap.SetSpeedWindMPH(2.237f * mPropellerPitch * mRevolver.GetFrequency()); break;
    case 17: mPixmap.Clear(); mPixmap.SetSpeedWindMPH(2.237f * mPropellerPitch * mRevolver.GetFrequency()); break;

    case 18: mPixmap.Clear(); mPixmap.SetSpiral(3, mPixmap.GetSizeX()/12, true); break;

    case 19: mPixmap.Clear(); mPixmap.SetWatch(); break;
    case 20: mPixmap.Clear(); mPixmap.SetWatch(); break;
    case 21: mPixmap.Clear(); mPixmap.SetWatch(); break;
    case 22: mPixmap.Clear(); mPixmap.SetWatch(); break;
    case 23: mPixmap.Clear(); mPixmap.SetWatch(); break;
    case 24: mPixmap.Clear(); mPixmap.SetWatch(); break;

    case 25: mPixmap.Clear(); mPixmap.SetStar(3); break;

    default: break;
  }
  vScreenIndex = (vScreenIndex < 25) ? vScreenIndex + 1 : 0;
}


/**
 * switch on only one LED (power safe at low rotation speeds)
 */
void BlinkLEDs()
{
  static byte vBladeIndex = 0;
  static byte vByte = 1;
  mBlades.Write((vBladeIndex+0)%3, vByte);
  mBlades.Write((vBladeIndex+1)%3, 0);
  mBlades.Write((vBladeIndex+2)%3, 0);
  vByte = vByte < 64 ? 2 * vByte : 1;
  if (vByte == 1) ++vBladeIndex;
}


/**
 * startup: init objects, configure IO-pins
 */
void setup()
{
#ifdef __DEBUG
  Serial.begin(115200);
  Serial.println();
  mPixmap.Test();
  Serial.end();
#endif // __DEBUG

  // configure LED pins
  mBlades.SetPins(0, mPinsBladeA);
  mBlades.SetPins(1, mPinsBladeB);
  mBlades.SetPins(2, mPinsBladeC);
  mBlades.Setup();
  
  // configure sensor pin
  mRevolver.Setup();
}


/**
 * perpetual main loop
 */
void loop()
{
  // check the revolution sensor and measure time
  mRevolver.Run();

  // if it's revolving well and fast enough ...
  if (mRevolver.IsOK() &&
     (mRevolver.GetMicros() < 1000000L)) {
    // ... show the current pixmap
    RenderBlades();    
    // swap pixmap, if time is due
    if (mSampler.IsDue()) {
      BrowseScreens();
    }    
  }
  else {
    // at low rotation speeds show single blinking LEDs
    if (mSampler.IsDue()) {
      BlinkLEDs();
    }
  }
}

