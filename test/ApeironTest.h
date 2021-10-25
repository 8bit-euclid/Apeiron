#pragma once

#include <gtest/gtest.h>
#include "../include/Global.h"

namespace Apeiron{

class ApeironTest : public testing::Test
{
public:
  Random<int> RandomInt;
  Random<Float> RandomFloat;

  ApeironTest() : RandomInt(-10, 10), RandomFloat(-Ten, Ten) {}

//  void SetUp() override
//  {
//
//  }
};

}
