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
  StaticArray<Float, 3> vect1;
//  StaticArray<Float, 3> vect2{One, Two, Three};
  std::array<Float, 3> vect3{One, Two, Three};

  constexpr auto test = StaticArraySupport::InitializeStaticArray<Float, 3>(Four);
  constexpr Float testValue = test[0];

  Print(__cplusplus);
//  Print(vect2[0], vect2[1], vect2[2]);
  Print(vect3[0], vect3[1], vect3[2]);
  Print(test[0], test[1], test[2]);
  Print(testValue);
//  StaticArray<Float, 3> vectSS;
//  StaticArray<Float, 4> vectSSS;
////  DynamicArray<Float> vectD(3);
//
////  isBounded(1, 0, (int)vectD.size());
//
//  vectSS[0] = 1.0;
//  vectSS[1] = 2.0;
//  vectSS[2] = 3.0;
//  Print(vectSS[2]);
//
//  vectS[0] = 10.0;
//  vectS[1] = 20.0;
//  vectS[2] = 30.0;
//
//
////  vectS = Two;
////  vectS = vectSS;
//  vectS = {21.0, 32.0};
////  vectS = vectSSS;
//  Print(vectS[2]);
//  Print(vectSS[2]);
//  Print(vectS != vectSS);

//  TestClass<bool> testBool1;
//  TestClass<int> testInt1;
//  TestClass<UInt> testInt2;
//  TestClass<Float> testDouble;
//
//  Print(testBool1.Value, testInt1.Value, testInt2.Value, testDouble.Value);

  return 0;
}