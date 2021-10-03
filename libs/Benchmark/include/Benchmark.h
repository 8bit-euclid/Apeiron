#pragma once

#include "../../../include/Global.h"
#include "../../Array/include/Array.h"
#include "Timer.h"

namespace Apeiron{

/** Benchmark class. */
class Benchmark
{
  private:
  TimeUnit TimeUnits;
  std::unordered_map<std::string, StopWatch> StopWatchMap;

  public:
  Benchmark(const TimeUnit& _time_units = TimeUnit::MilliSecond) : TimeUnits(_time_units) {};
  ~Benchmark() = default;

  inline void StartTimer(const std::string& _timer_name)
  {
    if(!StopWatchMap.count(_timer_name)) StopWatchMap.insert({_timer_name, StopWatch()});
    else ASSERT(!StopWatchMap[_timer_name].isRunning, "The timer for ", _timer_name, " is already running.")

    StopWatchMap[_timer_name].StartTimer();
  }

  inline void PauseTimer(const std::string& _timer_name)
  {
    DEBUG_ASSERT(StopWatchMap.count(_timer_name), "The timer for ", _timer_name, " has not yet been created.")
    DEBUG_ASSERT(StopWatchMap[_timer_name].isRunning, "The timer for ", _timer_name, " had not been started.")

    StopWatchMap[_timer_name].StopTimer();
  }

  inline void ResumeTimer(const std::string& _timer_name)
  {
    ASSERT(StopWatchMap.count(_timer_name), "The timer for ", _timer_name, " has not yet been created.")
    ASSERT(!StopWatchMap[_timer_name].isRunning, "The timer for ", _timer_name, " is already running.")

    StopWatchMap[_timer_name].StartTimer();
  }

  inline void StopTimer(const std::string& _timer_name)
  {
    DEBUG_ASSERT(StopWatchMap.count(_timer_name), "The timer for ", _timer_name, " has not yet been created.")

    PauseTimer(_timer_name);
    StopWatchMap[_timer_name].Accumulate(TimeUnits);
  }

  inline void PrintResultsHeader()
  {
    Print("***********************************************************************************************************");
    Print("|", Setw(15), "Timer Name", "|", Setw(10), " Lap Count ", "|", Setw(10), " Min Lap Time ", "|", Setw(10), " Max Lap Time ", "|", Setw(10),
          " Mean Lap Time ", "|", Setw(10), " RMS Lap Time ", "|", " Standard Deviation ", "|");
//    Print(" Timer Name | Lap Count | Min Lap Time | Max Lap Time | Mean Lap Time | RMS Lap Time | Standard Deviation |");
    Print("***********************************************************************************************************");
  }

  inline void PrintResults(const std::string& _timer_name, const bool is_print_header = false)
  {
    Finalise(_timer_name);
    if(is_print_header) PrintResultsHeader();
    Print("|", Setw(15), _timer_name, "|", Setw(10), StopWatchMap[_timer_name].LapCount, "|", Setw(10), StopWatchMap[_timer_name].LapTimeMin, "|", Setw(10),
          StopWatchMap[_timer_name].LapTimeMax, "|", Setw(10), StopWatchMap[_timer_name].LapTimeMean, "|", Setw(10), StopWatchMap[_timer_name].LapTimeRMS, "|",
          Setw(10), StopWatchMap[_timer_name].LapTimeStd, "|");
  }

  inline void PrintResults()
  {
    PrintResultsHeader();
    FOR_EACH(stop_watch, StopWatchMap) PrintResults(stop_watch.first);
    Print("***********************************************************************************************************");
  }

  private:
  inline void Finalise(const std::string& _timer_name)
  {
    DEBUG_ASSERT(StopWatchMap.count(_timer_name), "The timer for ", _timer_name, " has not yet been created.")
    StopWatchMap[_timer_name].Finalise();
  }
};

}