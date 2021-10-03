// main.cpp

#include <iostream>

#include "../include/Global.h"
#include "../libs/Benchmark/include/Benchmark.h"
#include "../libs/Benchmark/include/Timer.h"
#include <Array/include/Array.h>

using namespace Apeiron;

int main()
{
  Benchmark bm;
  FOR(i, 100000)
  {
    bm.StartTimer("Block 1");
    Random rd(-Ten, Ten);
    std::vector<Float> vect(1e3);
    FOR(i, 1e3) vect[i] = rd();
    bm.StopTimer("Block 1");

    bm.StartTimer("Block 2");
    std::vector<Float> vect1(1e3);
    FOR(i, 1e3) vect1[i] = rd();
    bm.PauseTimer("Block 2");

    bm.ResumeTimer("Block 2");
    FOR(i, 1e3) vect1[i] = rd();
    bm.StopTimer("Block 2");
    bm.PrintResults();
  }


  return 0;
}