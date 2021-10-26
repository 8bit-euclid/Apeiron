// main.cpp

#include <iostream>

#include "../include/Global.h"
#include "../libs/Benchmark/include/Benchmark.h"
#include "../libs/Benchmark/include/Timer.h"
#include "../libs/Functional/include/Explicit.h"
#include <DataContainers/include/Array.h>
#include <DataContainers/include/MultiArray.h>
#include <Tensor/include/Tensor.h>

using namespace Apeiron;

int main()
{
  Benchmark prof(TimeUnit::MicroSecond);
  Random<Float> rand(-1.0e3, 1.0e3);
  FOR(iter, 1000000)
  {
    prof.StartTimer("Arr1Create");
    StaticMultiDimArray<Float, 40, 25, 35> tensor1(Zero);
    prof.StopTimer("Arr1Create");

    prof.StartTimer("Arr1Assign");
    FOR(i, 40) FOR(j, 25) FOR(k, 35) tensor1(i, j, k) = rand();
    prof.StopTimer("Arr1Assign");

    prof.StartTimer("Arr1Access");
    FOR(i, 40) FOR(j, 25) FOR(k, 35) Float entry1 = rand()*tensor1(i, j, k);
    prof.StopTimer("Arr1Access");


    prof.StartTimer("Arr2Create");
    StaticMultiArray<Float, 40, 25, 35> tensor2(Zero);
    prof.StopTimer("Arr2Create");

    prof.StartTimer("Arr2Assign");
    FOR(i, 40) FOR(j, 25) FOR(k, 35) tensor2(i, j, k) = rand();
    prof.StopTimer("Arr2Assign");

    prof.StartTimer("Arr2Access");
    FOR(i, 40) FOR(j, 25) FOR(k, 35) Float entry2 = rand()*tensor2(i, j, k);
    prof.StopTimer("Arr2Access");

    prof.PrintResults();
  }

//  constexpr StaticMultiDimArray<Float, 3, 4, 5> tensor(One);
//  constexpr Float entry = tensor(1, 2, 3);


//  constexpr StaticMultiDimArray<double, 4, 5, 6> matrix2;
////  DynamicMultiDimArray<double> matrix2;
////  matrix2.Resize(2, 3, 4);
//
////  Print(matrix2.nEntries);
//  Print(std::get<0>(matrix.Dimensions), std::get<1>(matrix.Dimensions), std::get<2>(matrix.Dimensions));
//  Print(std::get<0>(matrix2.Dimensions), std::get<1>(matrix2.Dimensions), std::get<2>(matrix2.Dimensions));
}