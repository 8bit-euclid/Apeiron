#pragma once

#include "../../../include/Global.h"
#include "Timer.h"
#include <DataContainers/include/Array.h>

namespace Apeiron{

/** Benchmark class. */
class Benchmark
{
private:
  const int MaxStringLength;
  TimeUnit TimeUnits;
  std::unordered_map<std::string, StopWatch> StopWatchMap;

public:
  Benchmark(const TimeUnit& _time_units = TimeUnit::MilliSecond) : MaxStringLength(20), TimeUnits(_time_units) {};
  ~Benchmark() = default;

  /*************************************************************************************************************************************************************
  * Time Benchmarking Functions
  *************************************************************************************************************************************************************/
  /** Start the timer for a particular stopwatch. */
  inline void StartTimer(const std::string& _timer_name)
  {
    if(!StopWatchMap.count(_timer_name)) StopWatchMap.insert({_timer_name, StopWatch()});
    else ASSERT(!StopWatchMap[_timer_name].isRunning, "The timer for ", _timer_name, " is already running.")

    StopWatchMap[_timer_name].Start();
  }

  /** Pause the timer for a particular stopwatch. */
  inline void PauseTimer(const std::string& _timer_name)
  {
    DEBUG_ASSERT(StopWatchMap.count(_timer_name), "The timer for ", _timer_name, " has not yet been created.")
    DEBUG_ASSERT(StopWatchMap[_timer_name].isRunning, "The timer for ", _timer_name, " had not been started.")

    StopWatchMap[_timer_name].Stop();
  }

  /** Resume the timer for a particular stopwatch. */
  inline void ResumeTimer(const std::string& _timer_name)
  {
    ASSERT(StopWatchMap.count(_timer_name), "The timer for ", _timer_name, " has not yet been created.")
    ASSERT(!StopWatchMap[_timer_name].isRunning, "The timer for ", _timer_name, " is already running.")

    StopWatchMap[_timer_name].Start();
  }

  /** Stop the timer for a particular stopwatch. */
  inline void StopTimer(const std::string& _timer_name)
  {
    PauseTimer(_timer_name);
    StopWatchMap[_timer_name].AccumulateLapTimes(TimeUnits);
  }

  /** Print time benchmark result table header. */
  inline void PrintResultsHeader()
  {
    Print();
    Print("********************************************************************************************************************************");
    Print<'\0'>(Setw(MaxStringLength), " Timer Name ",
                "|", Setw(11), " Lap Count ",
                "|", Setw(17), "  Min Lap Time  ",
                "|", Setw(17), "  Max Lap Time  ",
                "|", Setw(17), "  Mean Lap Time  ",
                "|", Setw(17), "  RMS Lap Time  ",
                "|", Setw(22), "  Standard Deviation  ", "|");
    Print("********************************************************************************************************************************");
  }

  /** Print time benchmark result table. */
  inline void PrintResults(const std::string& _timer_name, const bool is_print_header = false)
  {
    Finalise(_timer_name);
    if(is_print_header) PrintResultsHeader();
    SetFormat(PrintFormat::Scientific);
    SetPrecision(3);
    Print<'\0'>(" ", Setw(MaxStringLength - 1), std::left, _timer_name,
                "|    ", Setw(7), StopWatchMap[_timer_name].LapTimes.size(),
                "|    ", Setw(13), StopWatchMap[_timer_name].LapTimeMin,
                "|    ", Setw(13), StopWatchMap[_timer_name].LapTimeMax,
                "|    ", Setw(13), StopWatchMap[_timer_name].LapTimeMean,
                "|    ", Setw(13), StopWatchMap[_timer_name].LapTimeRMS,
                "|    ", Setw(18), StopWatchMap[_timer_name].LapTimeStd, "|");
  }

  /** Print a single row of the time benchmark result table. */
  inline void PrintResults()
  {
    PrintResultsHeader();
    FOR_EACH(stop_watch, StopWatchMap) PrintResults(stop_watch.first);
    Print("********************************************************************************************************************************");
    FOR_EACH(stop_watch, StopWatchMap) stop_watch.second.Reset();
  }

  private:
  /** Finalise time metrics for the current lap. */
  inline void Finalise(const std::string& _timer_name)
  {
    DEBUG_ASSERT(StopWatchMap.count(_timer_name), "The timer for ", _timer_name, " has not yet been created.")
    StopWatchMap[_timer_name].FinaliseLap();
  }
};

}