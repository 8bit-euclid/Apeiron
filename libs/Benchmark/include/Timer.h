#pragma once

#include "../../../include/Global.h"
#include "../../Array/include/Array.h"

namespace Apeiron{

/** Units for measuring time. */
enum class TimeUnit
{
  NanoSecond,
  MicroSecond,
  MilliSecond,
  Second
};

/** Timer class. */
class Timer
{
  private:
  bool isRunning;
  Float TotalNanoSecondsLapsed;
  std::chrono::time_point<std::chrono::high_resolution_clock> StartTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> StopTime;

  friend class StopWatch;
  friend class Benchmark;

  public:
  Timer() : isRunning(false) {}
  ~Timer() = default;

  /** Start timer. */
  inline void StartTimer()
  {
    isRunning = true;
    StartTime = std::chrono::high_resolution_clock::now();
  }

  /** Stop timer and accumulate total time. */
  inline void StopTimer()
  {
    StopTime = std::chrono::high_resolution_clock::now();
    TotalNanoSecondsLapsed += std::chrono::time_point_cast<std::chrono::nanoseconds>(StopTime).time_since_epoch().count() -
                              std::chrono::time_point_cast<std::chrono::nanoseconds>(StartTime).time_since_epoch().count();
    isRunning = false;
  }

  /** Reset timer. */
  inline void ResetTimer()
  {
    *this = Timer();
  }

  /** Get total time lapsed. */
  inline Float TotalTimeLapsed(const TimeUnit& _time_units = TimeUnit::MilliSecond) const
  {
    switch(_time_units)
    {
      case TimeUnit::NanoSecond: return TotalNanoSecondsLapsed;
      case TimeUnit::MicroSecond: return 1.0e-3 * TotalNanoSecondsLapsed;
      case TimeUnit::MilliSecond: return 1.0e-6 * TotalNanoSecondsLapsed;
      case TimeUnit::Second: return 1.0e-9 * TotalNanoSecondsLapsed;
      default: ERROR("Time unit not recognised.")
    }
  }
};

/** Stopwatch class. */
class StopWatch : public Timer
{
  public:
  bool isFinalised;
  ULInt LapCount;
  Float LapTimeMin;
  Float LapTimeMax;
  Float LapTimeMean;
  Float LapTimeRMS;
  Float LapTimeStd;

  DynamicArray<Float> LapTimes;

  StopWatch() : isFinalised(false), LapCount(0), LapTimeMin(FloatMax), LapTimeMax(FloatLowest), LapTimeMean(Zero), LapTimeRMS(Zero) { LapTimes.reserve(1e5); }
  ~StopWatch() = default;

  inline void Accumulate(const TimeUnit& _time_units = TimeUnit::MilliSecond)
  {
    ASSERT(!isRunning, "Cannot accumulate - the timer is still running.")

    const Float total_time_lapsed = TotalTimeLapsed(_time_units);
    LapCount++;
    LapTimeMin = Min(LapTimeMin, total_time_lapsed);
    LapTimeMax = Max(LapTimeMax, total_time_lapsed);
    LapTimeMean += total_time_lapsed;
    LapTimeRMS += iPow(total_time_lapsed, 2);
    LapTimes.push_back(total_time_lapsed);
  }

  inline void Finalise()
  {
    ASSERT(!isRunning, "Cannot finalise - the timer is still running.")

    if(isFinalised) return;

    // Finalise the mean and RMS.
    LapTimeMean /= (Float)LapCount;
    LapTimeRMS = Sqrt(LapTimeRMS/(Float)LapCount);

    // Compute standard deviation from the mean and RMS.
    LapTimeStd = Zero;
    FOR_EACH(lap_time, LapTimes) LapTimeStd += LapTimeMean - Two*lap_time;
    LapTimeStd *= LapTimeMean/(Float)LapCount;
    LapTimeStd += LapTimeRMS;
    LapTimeStd = Sqrt(LapTimeStd);

    isFinalised = true;
  }
};

}
