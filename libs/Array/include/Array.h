#include "../../../include/Global.h"

#ifndef ARRAY_H
#define ARRAY_H

template <class class_name>
class DynamicArray : public std::vector<class_name>
{
  public:
  DynamicArray() = default;
  ~DynamicArray() = default;
};

template <class class_name, int vector_size>
class StaticArray
{
  class_name Data[vector_size];

  public:
  StaticArray() = default;
  ~StaticArray() = default;
};

#endif //ARRAY_H
