// main.cpp

#include <iostream>

#include "../include/Global.h"
#include <Array/include/Array.h>

using namespace Apeiron;

int main()
{
  SetFormat(PrintFormat::Scientific);
  SetPrecision(30);
  Print(1, 2, 3);
  Print(Divide(4, 3));
  Print(Modulo(4, 3));

  Random rdInt(-1, 4);
  Random rdFloat(-1.0, 4.0);
  Print(rdInt.Get(), rdFloat.Get());
//  Print(Abs(Exp(1) - e), Small, "isEqual:", isEqual(Abs(Exp(1) - e), Zero));

  return 0;
}