// main.cpp

#include <iostream>

#include "../include/Global.h"
#include <Array/include/Array.h>

using namespace Apeiron;

void Foo()
{
  DynamicArray<int> vect;
  vect.resize(3);
  Apeiron::PRINT_STACK(3);
}

int main()
{
  DynamicArray<int> array0;
  DynamicArray<int> array1(5);
  DynamicArray<int> array2(5, 100);
  DynamicArray<int> array3(array2.begin() + 1, array2.end());
  DynamicArray<int> array4 = {1, 2, 3};
  array4[3];

  StaticArray<int, 0> sarray0;
  StaticArray<int, 10> sarray1;
  sarray1[10];

  return 0;
}