#ifndef _GG_SAMPLER_H__
#define _GG_SAMPLER_H__

#include <Arduino.h>

class ggSampler
{
  
public:
  
  ggSampler(float aSamplesPerSecond);
  
  bool IsDue();
  unsigned long GetLastSampleDeltaMicroSeconds() const;
  float GetLastSampleDeltaSeconds() const;
  
private:
  
  const unsigned long mMicroSecondsSamplingTime;
  unsigned long mMicroSecondsLastSample;
  unsigned long mMicroSecondsDelta;
  
};

#endif // _GG_SAMPLER_H__
