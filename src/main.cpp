// main.cpp

#include <iostream>

#include "../include/Global.h"
#include <Array/include/Array.h>

using namespace Apeiron;

int main()
{
  SetFormat(PrintFormat::Scientific);
  SetPrecision(2);
  Print(1, 2, 3);
  Print(Divide(4, 3));
  Print(Modulo(4, 3));
  return 0;
}