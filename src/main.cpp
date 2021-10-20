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
//  constexpr StaticArray<Float, 3> vect1(Five);
//  constexpr StaticArray<Float, 3> vect2(vect1);
  constexpr StaticArray<Float, 3> vect3({One, Two, Three});

//  constexpr auto test = DataContainersSupport::InitStaticArray<Float, 3>({One, Two, Three});
//  constexpr Float testValue = test[0];

//  Print(test[0], test[1], test[2]);
//  Print(vect2[0], vect2[1], vect2[2]);
  Print(vect3[0], vect3[1], vect3[2]);
  return 0;
}