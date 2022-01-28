#pragma once

#include <gtest/gtest.h>
#include "../include/Global.h"

namespace Apeiron{

class ApeironTest : public testing::Test
{
public:
  Random<int> RandomInt;
  Random<Float> RandomFloat;

  std::array<int, 100> IntArray;
  std::array<Float, 100> FloatArray;

  ApeironTest()
    : RandomInt(-10, 10), RandomFloat(-Ten, Ten) {}

  void SetUp() override
  {
    FOR_EACH(entry, IntArray) entry = RandomInt();
    FOR_EACH(entry, FloatArray) entry = RandomFloat();
  }
};

}
