// main.cpp

#include <iostream>

#include "../include/Global.h"
#include <Array/include/Array.h>

void Foo()
{
  DynamicArray<int> vect;
  vect.resize(3);
  Apeiron::PRINT_STACK(3);
}

int main()
{
//  FOR(i, 2) std::cout<<"Hello World!\n";
  std::cout<<"Hello World: "<<ARGUMENT_COUNT()<<"\n";
  std::cout<<"Hello World: "<<ARGUMENT_COUNT(x)<<"\n";
  std::cout<<"Hello World: "<<ARGUMENT_COUNT(x, y)<<"\n";
  std::cout<<"Hello World: "<<ARGUMENT_COUNT(x, y, z)<<"\n";
  std::cout<<"Hello World: "<<ARGUMENT_COUNT(x, y, z, w)<<"\n";

//  if(0) 1;
//  else if(1) FOR_REVERSE(ii, 10, 8) ERROR("Hello World!!!")

  Foo();

  return 0;
}