// main.cpp

#include <iostream>

#include "../include/Global.h"
#include "../libs/Benchmark/include/Benchmark.h"
#include "../libs/Benchmark/include/Timer.h"
#include "../libs/Functional/include/Explicit.h"
#include <DataContainers/include/Array.h>
#include <Tensor/include/Tensor.h>

using namespace Apeiron;

int main()
{
  DynamicArray<Float> vect1(3, Four);
  DynamicArray<Float> vect2 = vect1;
  DynamicArray<Float> vect3 = {Two, Three, Four};
  DynamicArray<Float> vect4(vect3.begin(), vect3.end());

  Print(vect1[0], vect1[1], vect1[2]);
  Print(vect2[0], vect2[1], vect2[2]);
  Print(vect3[0], vect3[1], vect3[2]);
  Print(vect4[0], vect4[1], vect4[2]);
  return 0;
}