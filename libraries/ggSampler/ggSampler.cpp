#include <ggSampler.h>


ggSampler::ggSampler(float aSamplesPerSecond)
: mMicroSecondsSamplingTime(1000000.0f / aSamplesPerSecond),
  mMicroSecondsLastSample(0),
  mMicroSecondsDelta(0)
{
}


bool ggSampler::IsDue()
{
  unsigned long vMicroSeconds = micros();
  unsigned long vMicroSecondsDelta = vMicroSeconds - mMicroSecondsLastSample;
  if (vMicroSecondsDelta >= mMicroSecondsSamplingTime) {
    mMicroSecondsDelta = vMicroSecondsDelta;
    mMicroSecondsLastSample = vMicroSeconds;
    return true;
  }
  else {
    return false;
  }
}


unsigned long ggSampler::GetLastSampleDeltaMicroSeconds() const
{
  return mMicroSecondsDelta;
}


float ggSampler::GetLastSampleDeltaSeconds() const
{
  return 0.000001f * mMicroSecondsDelta;
}
