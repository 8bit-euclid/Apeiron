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
  StaticArray<bool, 3> boolTest;
  StaticArray<Bool, 3> BoolTest;
  StaticArray<int, 3> intTest;
  StaticArray<double, 3> doubleTest;

  Print(boolTest[0], BoolTest[0], intTest[0], doubleTest[0]);

//  constexpr TypeGroup boolTest = GetTypeGroup<bool>();
//  constexpr TypeGroup BoolTest = GetTypeGroup<Bool>();
//  constexpr TypeGroup intTest = GetTypeGroup<int>();

//  InitTypeValue<bool>();
//  InitTypeValue<Bool>();
//  InitTypeValue<int>();
//  Print(, GetTypeGroup<Bool>(), GetTypeGroup<int>());


//  Test<bool> boolTest;
//  Test<Bool> BoolTest; // This does not compile
//  Test<int> intTest;
//  Test<double> floatTest;
}