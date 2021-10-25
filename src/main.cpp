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
  StaticMultiArray<Float, 3, 4, 5> tensor(One);
  Float entry = tensor(1, 2, 3);
  tensor(1, 2, 3) = 2;
  Print(tensor(1, 2, 3));

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